// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogItemChest.h"



ARogItemChest::ARogItemChest()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	SetRootComponent(BaseMeshComp);
	BaseMeshComp->SetCollisionProfileName("InteractableItem");

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComp->SetupAttachment(GetRootComponent());
	LidMeshComp->SetCollisionProfileName("NoCollision");
}

void ARogItemChest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentPitch = FMath::FInterpConstantTo(CurrentPitch, RotationPitch, DeltaSeconds, RotationSpeed);
	LidMeshComp->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));

	if (FMath::IsNearlyEqual(CurrentPitch, RotationPitch))
	{
		UE_LOG(LogTemp, Error, TEXT("Pitch reached %f"), CurrentPitch);
		SetActorTickEnabled(false);
		BP_InteractionFinished();
	}
}

void ARogItemChest::Interact_Implementation()
{
	SetActorTickEnabled(true);
}

void ARogItemChest::BeginPlay()
{
	Super::BeginPlay();

	CurrentPitch = LidMeshComp->GetRelativeRotation().Pitch;
}

