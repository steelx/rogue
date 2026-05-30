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

	// Find the first valid Player Pawn on the server safely
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (APawn* PlayerPawn = PC->GetPawn())
			{
				GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), PlayerPawn);
				break; // Found our target, exit the loop
			}
		}
	}
}
