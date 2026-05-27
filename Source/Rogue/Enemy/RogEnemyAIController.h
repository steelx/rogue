// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RogEnemyAIController.generated.h"

UCLASS()
class ROGUE_API ARogEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogEnemyAIController();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Rogue|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
