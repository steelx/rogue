// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTT_HealSelf.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "AbilitySystem/RogAttributeSet.h"


EBTNodeResult::Type URogBTT_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* OwnerPawn = AIC->GetPawn();
	if (!OwnerPawn) return EBTNodeResult::Failed;

	// Use GAS to heal the enemy directly
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerPawn))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			// Get current health and max health to prevent over-healing
			const float CurrentHealth = ASC->GetNumericAttribute(URogAttributeSet::GetHealthAttribute());
			const float MaxHealth = ASC->GetNumericAttribute(URogAttributeSet::GetMaxHealthAttribute());

			UE_LOG(LogTemp, Error, TEXT("AI Healing CurrentHealth: %f, MaxHealth: %f"), CurrentHealth, MaxHealth);
			if (!FMath::IsNearlyZero(CurrentHealth) && CurrentHealth < MaxHealth)
			{
				const float ClampedHeal = FMath::Min(HealAmount, MaxHealth - CurrentHealth);
				// Directly add to the Health attribute
				ASC->ApplyModToAttribute(URogAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, ClampedHeal);

				UE_LOG(LogTemp, Warning, TEXT("Enemy healed. New Health: %f"), ClampedHeal);
			}

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Succeeded;
}
