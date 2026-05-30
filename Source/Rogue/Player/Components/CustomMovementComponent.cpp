// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "CustomMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


UCustomMovementComponent::UCustomMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceClimbableSurfaces();
	TraceFromEyeHeight(100.f, 15.f);
}

void UCustomMovementComponent::TraceClimbableSurfaces()
{
	const FVector StartOffset = UpdatedComponent->GetForwardVector() * 30.f;
	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector();

	DoCapsuleTraceMultiByObject(Start, End, true);
}

void UCustomMovementComponent::TraceFromEyeHeight(const float TraceDistance, const float StartOffset)
{
	const FVector Location = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + StartOffset);
	const FVector Start = Location + EyeHeightOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;

	DoLineTraceSingleByObject(Start, End, true);
}

TArray<FHitResult> UCustomMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, const bool bShowDebugShape) const
{
	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutCapsuleTrace;

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this, Start, End, ClimbCapsuleTraceRadius, ClimbCapsuleTraceHalfHeight, ClimbSurfaceTraceTypes,
		false, ActorsToIgnore, bShowDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		OutCapsuleTrace, true);

	return OutCapsuleTrace;
}

FHitResult UCustomMovementComponent::DoLineTraceSingleByObject(const FVector& Start, const FVector& End, const bool bShowDebugShape) const
{
	const TArray<AActor*> ActorsToIgnore;
	FHitResult OutLineTrace;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End, ClimbSurfaceTraceTypes, false, ActorsToIgnore,
		bShowDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, OutLineTrace, true);

	return OutLineTrace;
}
