// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTS_IsLowHealth.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "AbilitySystem/RogAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"

void URogBTS_IsLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return;

	APawn* OwnerPawn = AIC->GetPawn();
	if (!OwnerPawn) return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerPawn))
	{
		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
		if (ensure(ASC))
		{
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

			const float Health = ASC->GetNumericAttribute(URogAttributeSet::GetHealthAttribute());
			BBComp->SetValueAsFloat(CurrentHealthKey.SelectedKeyName, Health);
		}
	}
}
