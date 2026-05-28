// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogAbilitySystemComponent.h"

#include "Core/MyTags.h"


URogAbilitySystemComponent::URogAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

FGameplayEffectSpecHandle URogAbilitySystemComponent::MakePlayerDamageEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, float InBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount) const
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	// ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetOwnerActor());
	ContextHandle.AddInstigator(GetOwnerActor(),GetOwnerActor());

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(MyTags::Shared_SetByCaller_BaseDamage, InBaseDamage);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
	}

	return EffectSpecHandle;
}

