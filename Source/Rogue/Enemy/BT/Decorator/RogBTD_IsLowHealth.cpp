// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTD_IsLowHealth.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Core/Interfaces/RogActionSystemInterface.h"

bool URogBTD_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return false;

	APawn* OwnerPawn = AIC->GetPawn();
	if (!OwnerPawn) return false;

	if (const IRogActionSystemInterface* ASI = Cast<IRogActionSystemInterface>(OwnerPawn))
	{
		if (const URogueActionSystemComponent* ActionSystemComponent = ASI->GetActionSystemComponent())
		{
			const float Health = ActionSystemComponent->GetAttributesSet().Health;
			const float HealthMax = ActionSystemComponent->GetAttributesSet().HealthMax;
			return (Health/HealthMax) < LowHealthFraction;
		}
	}

	return false;
}
