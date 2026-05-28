// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RogBTT_SmoothRotate.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_API URogBTT_SmoothRotate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URogBTT_SmoothRotate();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AcceptanceRadius; // Degrees of tolerance

private:
	TSoftObjectPtr<AActor> CachedTargetActor;
};
