// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "CustomMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Core/RogGameFunctions.h"
#include "GameFramework/Character.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"



UCustomMovementComponent::UCustomMovementComponent()
{
	bOrientRotationToMovement = true;
	bUseControllerDesiredRotation = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void UCustomMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	if (OwningPlayerAnimInstance)
	{
		OwningPlayerAnimInstance->OnMontageEnded.AddDynamic(this, &UCustomMovementComponent::OnMontageEnded);
		OwningPlayerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &UCustomMovementComponent::OnMontageEnded);
	}
}

void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	const float HalfHeightDelta = DefaultCapsuleHalfHeight - ClimbCapsuleHalfHeight;

	if (IsClimbing())
	{
		bOrientRotationToMovement = false;
		bUseControllerDesiredRotation = false;
		CharacterOwner->bUseControllerRotationYaw = false;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(ClimbCapsuleHalfHeight);

		// Shift mesh down so the smaller capsule sits higher (upper-body aligned)
		CharacterOwner->GetMesh()->AddLocalOffset(FVector(0.f, 0.f, -HalfHeightDelta));
	}

	// Just stopped Climbing
	else if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
	{
		bOrientRotationToMovement = true;
		CharacterOwner->bUseControllerRotationYaw = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight);

		// Restore mesh to its original position
		CharacterOwner->GetMesh()->AddLocalOffset(FVector(0.f, 0.f, HalfHeightDelta));

		const FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
		const FRotator CleanUpRotation = FRotator(0, DirtyRotation.Yaw, 0);
		UpdatedComponent->SetRelativeRotation(CleanUpRotation);
		StopMovementImmediately();
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UCustomMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	if (IsClimbing())
	{
		PhysicsClimb(deltaTime, Iterations);
	}

	Super::PhysCustom(deltaTime, Iterations);
}

float UCustomMovementComponent::GetMaxSpeed() const
{
	if (IsClimbing()) return MaxClimbSpeed;
	return Super::GetMaxSpeed();
}

float UCustomMovementComponent::GetMaxAcceleration() const
{
	if (IsClimbing()) return MaxClimbAcceleration;
	return Super::GetMaxAcceleration();
}

FVector UCustomMovementComponent::ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const
{
	// e.g. when we reach top we call StopClimbing which will fall player off ledge, we don't want that;
	// let Root Motion Montage to handle the Velocity instead
	const bool bIsPlayingRmMontage = IsFalling() && OwningPlayerAnimInstance && OwningPlayerAnimInstance->IsAnyMontagePlaying();
	if (bIsPlayingRmMontage) return RootMotionVelocity;

	return Super::ConstrainAnimRootMotionVelocity(RootMotionVelocity, CurrentVelocity);
}

void UCustomMovementComponent::ToggleClimbing(const bool bEnableClimb)
{
	if (bEnableClimb && CanStartClimbing())
	{
		PlayClimbMontage(IdleToClimbMontage);
		return;
	}

	StopClimbing();
	UE_LOG(LogTemp, Warning, TEXT("<-- Stopping Climbing! -->"));
}

void UCustomMovementComponent::RequestHopping()
{
}

FVector UCustomMovementComponent::GetUnrotatedClimbVelocity() const
{
	return UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), Velocity);
}

bool UCustomMovementComponent::IsClimbing() const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == ECustomMovementMode::MOVE_Climb;
}

bool UCustomMovementComponent::TraceClimbableSurfaces()
{
	const FVector StartOffset = UpdatedComponent->GetForwardVector() * 30.f;
	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector();

	ClimbableSurfacesTracedResults = DoCapsuleTraceMultiByObject(Start, End, false);

	return !ClimbableSurfacesTracedResults.IsEmpty();
}

FHitResult UCustomMovementComponent::DoTraceFromEyeHeight(const float TraceDistance, const float StartOffset, const bool bShowDebugShape)
{
	const FVector Location = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + StartOffset);
	const FVector Start = Location + EyeHeightOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;

	const FHitResult Hit = DoLineTraceSingleByObject(Start, End, bShowDebugShape);
	return Hit;
}

bool UCustomMovementComponent::CanStartClimbing()
{
	if (IsFalling()) return false;
	if (!TraceClimbableSurfaces()) return false;
	if (!DoTraceFromEyeHeight(100.f, 15.f).bBlockingHit) return false;

	return true;
}

void UCustomMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, ECustomMovementMode::MOVE_Climb);
	// UE_LOG(LogTemp, Warning, TEXT("Starting Climbing Movement Mode: MOVE_Climb"));
}

void UCustomMovementComponent::StopClimbing()
{
	SetMovementMode(MOVE_Falling);
}

void UCustomMovementComponent::PhysicsClimb(const float DeltaTime, const int32 Iterations)
{
	/// Copied from UCharacterMovementComponent::PhysFlying
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	/** 1. Process all climbable surfaces info **/
	TraceClimbableSurfaces();
	ProcessClimbableSurfaceInfo();

	/** 2. Check if we should stop Climbing **/
	if (CheckShouldStopClimbing() || CheckHasReachedFloor())
	{
		StopClimbing();
	}

	RestorePreAdditiveRootMotionVelocity();

	if( !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() )
	{
		CalcVelocity(DeltaTime, 0, true, MaxBreakClimbDeceleration);
	}

	ApplyRootMotionToVelocity(DeltaTime);

	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * DeltaTime;
	FHitResult Hit(1.f);

	// this moves the Character (+ Handle climb rotation)
	SafeMoveUpdatedComponent(Adjusted, GetClimbRotation(DeltaTime), true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	if(!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}

	/** 2. Snap movement to climbable surfaces **/
	SnapMovementToClimbableSurfaces(DeltaTime);

	if (CheckHasReachedTop())
	{
		StopClimbing();
		PlayClimbMontage(ClimbToTopMontage);
	}
}

void UCustomMovementComponent::ProcessClimbableSurfaceInfo()
{
	CurrentClimbableSurfaceLocation = FVector::ZeroVector;
	CurrentClimbableSurfaceNormal = FVector::ZeroVector;
	if (ClimbableSurfacesTracedResults.IsEmpty()) return;

	for (const FHitResult HitResult : ClimbableSurfacesTracedResults)
	{
		CurrentClimbableSurfaceLocation += HitResult.ImpactPoint;
		CurrentClimbableSurfaceNormal += HitResult.ImpactNormal;
	}

	CurrentClimbableSurfaceLocation /= ClimbableSurfacesTracedResults.Num();// get average location
	CurrentClimbableSurfaceNormal = CurrentClimbableSurfaceNormal.GetSafeNormal();
}

bool UCustomMovementComponent::CheckShouldStopClimbing() const
{
	if (ClimbableSurfacesTracedResults.IsEmpty()) return true;

	// when we are at Top Edge, the top of the cube is UpVector and the existing Climb Wall is the Surface Normal
	// If both Vectors are Up which is 90 deg angle their Dot product is 1.
	const float DotResult = FVector::DotProduct(CurrentClimbableSurfaceNormal, FVector::UpVector);
	// Is the slope too steep? Acos(1) to Degrees = 0, desired is 90.
	const float DegreesSlope = FMath::RadiansToDegrees(FMath::Acos(DotResult));
	return DegreesSlope <= 45.f;
}

bool UCustomMovementComponent::CheckHasReachedFloor()
{
	const FVector Down = -UpdatedComponent->GetUpVector();
	const FVector StartOffset = Down * 50.f;
	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + Down;

	const TArray<FHitResult> PossibleFloorHits = DoCapsuleTraceMultiByObject(Start, End);

	if (PossibleFloorHits.IsEmpty()) return false;

	for (const FHitResult& Hit : PossibleFloorHits)
	{
		// Surface is horizontal (floor-like)
		const bool bIsHorizontalSurface = FVector::Parallel(-Hit.ImpactNormal, FVector::UpVector);
		if (!bIsHorizontalSurface) continue;

		// UE_LOG(LogTemp, Error, TEXT("Found a horizontal surface for the floor!"));
		// Moving downward & already stopped near the floor
		const bool bMovingDownward = GetUnrotatedClimbVelocity().Z < -10.f;
		if (bMovingDownward && bIsHorizontalSurface) return true;
	}

	return false;
}

bool UCustomMovementComponent::CheckHasReachedTop()
{
	const FHitResult LedgeResult = DoTraceFromEyeHeight(100.f, 50.f, true);
	const bool bMovingUpward = GetUnrotatedClimbVelocity().Z > 10.f;
	// False means no climbable Surface on Eye level
	if (!LedgeResult.bBlockingHit && bMovingUpward)
	{
		// 2nd Line trace from the End of HitResult to downwards
		const FVector WalkableSurfaceTraceStart = LedgeResult.TraceEnd;
		const FVector Down = -UpdatedComponent->GetUpVector();
		const FVector End = WalkableSurfaceTraceStart + Down*50.f;

		const FHitResult WalkableSurfaceHit = DoLineTraceSingleByObject(WalkableSurfaceTraceStart, End, true);
		if (WalkableSurfaceHit.bBlockingHit) return true;
	}

	return false;
}

FQuat UCustomMovementComponent::GetClimbRotation(const float DeltaTime) const
{
	const FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();
	const FQuat TargetQuat = FRotationMatrix::MakeFromX(-CurrentClimbableSurfaceNormal).ToQuat();

	if (HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity()) return TargetQuat;

	return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f);
}

void UCustomMovementComponent::SnapMovementToClimbableSurfaces(const float DeltaTime) const
{
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector ComponentForward = UpdatedComponent->GetForwardVector();
	const FVector ProjectedCharacterToSurface = (CurrentClimbableSurfaceLocation-ComponentLocation).ProjectOnTo(ComponentForward);
	const FVector SnapVector = -CurrentClimbableSurfaceNormal * ProjectedCharacterToSurface.Length();

	UpdatedComponent->MoveComponent(SnapVector*DeltaTime*MaxClimbSpeed, UpdatedComponent->GetComponentQuat(), true);
}

void UCustomMovementComponent::PlayClimbMontage(UAnimMontage* MontageToPlay) const
{
	if (!MontageToPlay || !OwningPlayerAnimInstance) return;
	if (OwningPlayerAnimInstance->IsAnyMontagePlaying()) return;

	OwningPlayerAnimInstance->Montage_Play(MontageToPlay);
}

void UCustomMovementComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == IdleToClimbMontage) StartClimbing();
	else SetMovementMode(MOVE_Walking);
}

TArray<FHitResult> UCustomMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, const bool bShowDebugShape, const bool bDrawPersistentShape)
{
	const TArray<AActor*>& ActorsToIgnore = { CharacterOwner };
	return URogGameFunctions::DoCapsuleTraceMultiByObject(this, Start, End, ClimbSurfaceTraceTypes, ActorsToIgnore, ClimbCapsuleTraceRadius, ClimbCapsuleHalfHeight, bShowDebugShape, bDrawPersistentShape);
}

FHitResult UCustomMovementComponent::DoLineTraceSingleByObject(const FVector& Start, const FVector& End, const bool bShowDebugShape, const bool bDrawPersistentShape)
{
	const TArray<AActor*>& ActorsToIgnore = { CharacterOwner };
	return URogGameFunctions::DoLineTraceSingleByObject(this, Start, End, ClimbSurfaceTraceTypes, ActorsToIgnore, bShowDebugShape, bDrawPersistentShape);
}
