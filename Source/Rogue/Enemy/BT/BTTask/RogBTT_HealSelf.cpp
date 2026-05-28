// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTT_HealSelf.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Core/Interfaces/RogActionSystemInterface.h"

EBTNodeResult::Type URogBTT_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* OwnerPawn = AIC->GetPawn();
	if (!OwnerPawn) return EBTNodeResult::Failed;

	if (IRogActionSystemInterface* ASI = Cast<IRogActionSystemInterface>(OwnerPawn))
	{
		URogueActionSystemComponent* ActionSystemComponent = ASI->GetActionSystemComponent();
		if (!ActionSystemComponent) return EBTNodeResult::Failed;

		ActionSystemComponent->ApplyHealthChange(HealAmount);
	}

	return EBTNodeResult::Succeeded;
}
