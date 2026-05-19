// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Engine/OverlapResult.h"
#include "Rogue/Interfaces/RogInteractableInterface.h"


URogInteractionComponent::URogInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URogInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	const FVector PawnCenterLoc = PC->GetPawn()->GetActorLocation();

	constexpr ECollisionChannel CollisionChannel = INTERACTION_TRACE_CHANNEL;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);

	// Debug draws
#if WITH_EDITOR
	if (bDrawDebugs) DrawDebugSphere(GetWorld(), PawnCenterLoc, InteractionRadius, 12, FColor::White);
#endif

	TArray<FOverlapResult> OutOverlaps;
	GetWorld()->OverlapMultiByChannel(OutOverlaps, PawnCenterLoc, FQuat::Identity, CollisionChannel, CollisionShape);

	// Interactable
	AActor* BestActor = nullptr;
	float HighestDotResult = -1;

	for (FOverlapResult& OverlapResult : OutOverlaps)
	{
		FVector OverlapLocation = OverlapResult.GetActor()->GetActorLocation();
		FVector OverlapDirection = (OverlapLocation - PawnCenterLoc).GetSafeNormal();
		const float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());

		if (DotResult > HighestDotResult)
		{
			HighestDotResult = DotResult;
			BestActor = OverlapResult.GetActor();
		}

		if (bDrawDebugs)
		{
			DrawDebugBox(GetWorld(), OverlapLocation, FVector(55.f), FColor::Red);
			DrawDebugString(GetWorld(), OverlapLocation, FString::Printf(TEXT("Dot: %f"), DotResult), nullptr, FColor::Cyan, 0,  true, 2.f);
		}
	}

	if (IsValid(BestActor))
	{
		SelectedActor = BestActor;
		if (bDrawDebugs)
		{
			DrawDebugDirectionalArrow(GetWorld(), PawnCenterLoc, BestActor->GetActorLocation(), 2.f, FColor::Green);
		}
	}
}

void URogInteractionComponent::Interact() const
{
	if (!SelectedActor.IsValid()) return;
	if (!SelectedActor.Get()->Implements<URogInteractableInterface>()) return;

	IRogInteractableInterface::Execute_Interact(SelectedActor.Get());
}

