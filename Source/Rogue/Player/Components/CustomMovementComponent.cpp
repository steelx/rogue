// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "CustomMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PhysicsVolume.h"
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
	if (IsClimbing())
	{
		bOrientRotationToMovement = false;
		bUseControllerDesiredRotation = false;
		CharacterOwner->bUseControllerRotationYaw = false;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
	}

	// Just stopped Climbing
	else if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
	{
		bOrientRotationToMovement = true;
		CharacterOwner->bUseControllerRotationYaw = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);

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
	return FVector::ZeroVector;
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

	ClimbableSurfacesTracedResults = DoCapsuleTraceMultiByObject(Start, End, true);

	return !ClimbableSurfacesTracedResults.IsEmpty();
}

bool UCustomMovementComponent::TraceFromEyeHeight(const float TraceDistance, const float StartOffset)
{
	const FVector Location = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + StartOffset);
	const FVector Start = Location + EyeHeightOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;

	const FHitResult Hit = DoLineTraceSingleByObject(Start, End);
	return Hit.bBlockingHit;
}

bool UCustomMovementComponent::CanStartClimbing()
{
	if (IsFalling()) return false;
	if (!TraceClimbableSurfaces()) return false;
	if (!TraceFromEyeHeight(100.f, 15.f)) return false;

	return true;
}

void UCustomMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, ECustomMovementMode::MOVE_Climb);
	UE_LOG(LogTemp, Warning, TEXT("Starting Climbing Movement Mode: MOVE_Climb"));
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
	if (CheckShouldStopClimbing())
	{
		StopClimbing();
	}

	RestorePreAdditiveRootMotionVelocity();

	if( !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() )
	{
		//TODO: Define max climb speed and acceleration
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
}

TArray<FHitResult> UCustomMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, const bool bShowDebugShape, const bool bDrawPersistentShape) const
{
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	if (bShowDebugShape)
	{
		DrawDebugType = bDrawPersistentShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::ForOneFrame;
	}
	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutCapsuleTrace;

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this, Start, End, ClimbCapsuleTraceRadius, ClimbCapsuleTraceHalfHeight, ClimbSurfaceTraceTypes,
		false, ActorsToIgnore, DrawDebugType, OutCapsuleTrace, true);

	return OutCapsuleTrace;
}

FHitResult UCustomMovementComponent::DoLineTraceSingleByObject(const FVector& Start, const FVector& End, const bool bShowDebugShape, const bool bDrawPersistentShape) const
{
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	if (bShowDebugShape)
	{
		DrawDebugType = bDrawPersistentShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::ForOneFrame;
	}
	const TArray<AActor*> ActorsToIgnore;
	FHitResult OutLineTrace;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End, ClimbSurfaceTraceTypes, false, ActorsToIgnore,
		DrawDebugType, OutLineTrace, true);

	return OutLineTrace;
}
