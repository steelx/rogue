// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RogBTS_CheckRangeTo.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_API URogBTS_CheckRangeTo : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "BTService")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "BTService")
	FBlackboardKeySelector MaxAttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "BTService")
	FBlackboardKeySelector WithinRangeKey;
};
