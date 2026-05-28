// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RogBTD_IsLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_API URogBTD_IsLowHealth : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category="BTDecorator", meta=(ClampMin="0.0", ClampMax="1.0"))
	float LowHealthFraction {0.3};
};
