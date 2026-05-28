// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RogBTD_RandomChance.generated.h"

/**
 *
 */
UCLASS()
class ROGUE_API URogBTD_RandomChance : public UBTDecorator
{
	GENERATED_BODY()

public:
	URogBTD_RandomChance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// Set the probability between 0.0 (0%) and 1.0 (100%)
	UPROPERTY(EditAnywhere, Category = "Condition", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SuccessProbability;
};
