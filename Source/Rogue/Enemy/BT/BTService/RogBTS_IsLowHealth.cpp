// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTS_IsLowHealth.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Interfaces/RogActionSystemInterface.h"

void URogBTS_IsLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return;

	APawn* OwnerPawn = AIC->GetPawn();
	if (!OwnerPawn) return;

	if (IRogActionSystemInterface* ASI = Cast<IRogActionSystemInterface>(OwnerPawn))
	{
		URogueActionSystemComponent* ActionSystemComponent = ASI->GetActionSystemComponent();
		if (ensure(ActionSystemComponent))
		{
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			BBComp->SetValueAsFloat(CurrentHealthKey.SelectedKeyName, ActionSystemComponent->GetAttributesSet().Health);
		}
	}
}
