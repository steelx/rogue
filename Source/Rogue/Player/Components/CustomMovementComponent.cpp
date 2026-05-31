// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "CustomMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (IsClimbing())
	{
		bOrientRotationToMovement = false;
		CharacterOwner->bUseControllerRotationYaw = false;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
	}

	// Just starting Climbing
	else if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
	{
		bOrientRotationToMovement = true;
		CharacterOwner->bUseControllerRotationYaw = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
		StopMovementImmediately();
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UCustomMovementComponent::ToggleClimbing(const bool bEnableClimb)
{
	if (!bEnableClimb)
	{
		// Stop Climb
		UE_LOG(LogTemp, Warning, TEXT("--> Stop Climbing <--"));
		StopClimbing();
		return;
	}

	if (CanStartClimbing())
	{
		// Start climb
		UE_LOG(LogTemp, Warning, TEXT("++> Starting Climbing <++"));
		StartClimbing();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("<-- Stopping Climbing! -->"));
	}
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

	ClimbableSurfacesTracedResults = DoCapsuleTraceMultiByObject(Start, End, true, true);

	return !ClimbableSurfacesTracedResults.IsEmpty();
}

bool UCustomMovementComponent::TraceFromEyeHeight(const float TraceDistance, const float StartOffset)
{
	const FVector Location = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + StartOffset);
	const FVector Start = Location + EyeHeightOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;

	const FHitResult Hit = DoLineTraceSingleByObject(Start, End, true, true);
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
}

void UCustomMovementComponent::StopClimbing()
{
	SetMovementMode(MOVE_Falling);
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
