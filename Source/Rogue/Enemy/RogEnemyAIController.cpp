// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


ARogEnemyAIController::ARogEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARogEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Error, TEXT("BehaviorTree is NULL"));
		return;
	}
	RunBehaviorTree(BehaviorTree);

	// TODO: this is temp, wont work on client
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	checkf(IsValid(PlayerPawn), TEXT("Player Pawn not found in AI Controller!"));

	GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), PlayerPawn);
}
