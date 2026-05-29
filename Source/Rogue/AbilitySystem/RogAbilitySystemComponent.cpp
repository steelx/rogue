// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogAbilitySystemComponent.h"

#include "Core/MyTags.h"


void URogAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;
	UE_LOG(LogTemp, Log, TEXT("OnAbilityInputPressed, InputTag: %s"), *InInputTag.ToString());

	ABILITYLIST_SCOPE_LOCK();

	// Snapshot matching handles first to avoid mutating the activatable list during iteration
	TArray<FGameplayAbilitySpecHandle> MatchingHandles;
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!IsValid(AbilitySpec.Ability)) continue;

		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			// convert const Ability Spec to Pointer FoundSpec
			FGameplayAbilitySpec* FoundSpec = FindAbilitySpecFromHandle(AbilitySpec.Handle);
			if (!FoundSpec) continue;

			if (FoundSpec->IsActive())
			{
				AbilitySpecInputPressed(*FoundSpec);
			}
			TryActivateAbility(FoundSpec->Handle);
		}
		// DO NOT break here! Allow the loop to check if other abilities share this input tag.
	}
}

void URogAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;
	UE_LOG(LogTemp, Log, TEXT("[UMyAbilitySystemComponent] OnAbilityInputReleased"));

	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!IsValid(AbilitySpec.Ability)) continue;
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			// THIS ability matches the tag we just released
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

FGameplayEffectSpecHandle URogAbilitySystemComponent::MakeDamageEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, float InBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount) const
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	// ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetOwnerActor());
	ContextHandle.AddInstigator(GetOwnerActor(),GetOwnerActor());

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(
		EffectClass, 1, ContextHandle
	);


	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
	}

	return EffectSpecHandle;
}

void URogAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	HandleAutoActivatedAbility(AbilitySpec);
}

void URogAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		HandleAutoActivatedAbility(AbilitySpec);
	}
}

void URogAbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!IsValid(AbilitySpec.Ability)) return;

	for (const FGameplayTag& Tag : AbilitySpec.Ability->GetAssetTags())
	{
		if (Tag.MatchesTagExact(MyTags::Abilities_ActivateOnGiven))
		{
			TryActivateAbility(AbilitySpec.Handle);
			return;
		}
	}
}
