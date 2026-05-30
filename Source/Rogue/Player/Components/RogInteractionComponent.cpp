// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogInteractionComponent.h"

#include "Core/RogueGameTypes.h"
#include "Core/Interfaces/RogInteractableInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false, TEXT("Enable debug drawing. (0 = off, 1 = enabled)"), ECVF_Cheat);

URogInteractionComponent::URogInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URogInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URogInteractionComponent::PerformInteractionTrace, TimerDelay, true);
}


void URogInteractionComponent::Interact() const
{
	if (!SelectedActor.IsValid()) return;
	if (!SelectedActor.Get()->Implements<URogInteractableInterface>()) return;

	IRogInteractableInterface::Execute_Interact(SelectedActor.Get());
}

void URogInteractionComponent::PerformInteractionTrace()
{
	const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	const FVector PawnCenter = PC->GetPawn()->GetActorLocation();
	const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

	constexpr ECollisionChannel CollisionChannel = INTERACTION_TRACE_CHANNEL;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);

	const bool bDrawDebugs = CVarInteractionDebugDrawing.GetValueOnGameThread();
	// Debug draws
	if (bDrawDebugs) DrawDebugSphere(GetWorld(), PawnCenter, InteractionRadius, 12, FColor::White, false, TimerDelay);

	TArray<FOverlapResult> OutOverlaps;
	GetWorld()->OverlapMultiByChannel(OutOverlaps, PawnCenter, FQuat::Identity, CollisionChannel, CollisionShape);

	// Interactable
	AActor* BestActor = nullptr;
	float HighestWeight = 0.f;

	for (FOverlapResult& OverlapResult : OutOverlaps)
	{
		// Get Object Bounds center, rather than center returned due to Mesh Pivot
		FVector Origin; FVector BoxExtent;
		OverlapResult.GetActor()->GetActorBounds(true, Origin, BoxExtent);

		// FVector OverlapLocation = OverlapResult.GetActor()->GetActorLocation();
		FVector OverlapDirection = (Origin - CameraLocation).GetSafeNormal();

		const float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());// -1 to 1
		const float NormalizedDotResult = DotResult * 0.5f + 0.5;// 0 to 1

		const float DistanceTo = (PawnCenter - Origin).Size();// 0 to Interaction Radius (400)
		const float NormalizedDistance = 1.0f - (DistanceTo / InteractionRadius);// 0 to 1 will become 1 to 0

		const float Weight = NormalizedDotResult + NormalizedDistance;
		if (Weight > HighestWeight)
		{
			HighestWeight = Weight;
			BestActor = OverlapResult.GetActor();
		}

		if (bDrawDebugs)
		{
			DrawDebugBox(GetWorld(), Origin, FVector(55.f), FColor::Red, false, TimerDelay);
			DrawDebugString(GetWorld(), Origin, FString::Printf(TEXT("Weight: %f"), Weight), nullptr, FColor::Cyan, TimerDelay,  true, 2.f);
		}
	}

	if (IsValid(BestActor))
	{
		SelectedActor = BestActor;
		if (bDrawDebugs)
		{
			DrawDebugDirectionalArrow(GetWorld(), PawnCenter, BestActor->GetActorLocation(), 2.f, FColor::Green, false, TimerDelay);
		}
	}
}

