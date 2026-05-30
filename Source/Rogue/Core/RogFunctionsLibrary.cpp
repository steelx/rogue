// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogFunctionsLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "MyTags.h"

FGameplayEffectSpecHandle URogFunctionsLibrary::MakeDamageEffectSpec(
	UAbilitySystemComponent* ASCWhoFired, TSubclassOf<UGameplayEffect> EffectClass,
	float InBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount, int32 Level)
{
	if (!IsValid(ASCWhoFired) || !IsValid(EffectClass))
	{
		return FGameplayEffectSpecHandle();
	}

	// 1. Build the context using the explicitly provided Actor
	AActor* ActorWhoFired = ASCWhoFired->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle = ASCWhoFired->MakeEffectContext();
	ContextHandle.AddSourceObject(ActorWhoFired);//Actor who fired

	// AddInstigator takes (Instigator, EffectCauser).
	// For a magic projectile, the Character is the Instigator.
	// To use held Weapons, the Weapon would be the EffectCauser. Here, we use the Actor for both.
	ContextHandle.AddInstigator(ActorWhoFired, ActorWhoFired);

	// 2. Generate the Spec
	FGameplayEffectSpecHandle EffectSpecHandle = ASCWhoFired->MakeOutgoingSpec(EffectClass, Level, ContextHandle);

	if (EffectSpecHandle.IsValid())
	{
		// 3. Inject dynamic SetByCaller data
		EffectSpecHandle.Data->SetSetByCallerMagnitude(MyTags::Shared_SetByCaller_BaseDamage, InBaseDamage);

		if (InCurrentAttackTypeTag.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
		}
	}

	return EffectSpecHandle;
}
