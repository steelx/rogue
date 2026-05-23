// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogPatrolPointManager.h"

#include "Components/BillboardComponent.h"


ARogPatrolPointManager::ARogPatrolPointManager()
{
	PrimaryActorTick.bCanEverTick = false;

	BillboardIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(BillboardIcon);
}

void ARogPatrolPointManager::BeginPlay()
{
	Super::BeginPlay();

	if (PatrolPoints.Num() > 0)
	{
		CurrentPatrolPointIndex = PatrolPoints.Num() - 1;
	}
}

ATargetPoint* ARogPatrolPointManager::GetNextPatrolPoint()
{
	if (PatrolPoints.Num() == 0) return nullptr;

	// Advance and wrap around
	CurrentPatrolPointIndex = (CurrentPatrolPointIndex + 1) % PatrolPoints.Num();

	return PatrolPoints[CurrentPatrolPointIndex];
}
