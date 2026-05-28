// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "DataAsset_StartupDataBase.h"

#include "GameplayEffect.h"
#include "AbilitySystem/RogAbilitySystemComponent.h"

void UDataAsset_StartupDataBase::GiveToAbilitySystemComponent(URogAbilitySystemComponent* InASC, int32 Level) const
{
	if (!InASC)
	{
		UE_LOG(LogTemp, Error, TEXT("InASC is null in UDataAsset_StartupData::GiveToAbilitySystemComponent"));
		return;
	}
	GrantAbilities(ActivateOnGivenAbilities, InASC, Level);
	GrantAbilities(ReactiveAbilities, InASC, Level);

	if (!StartupGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& GEClass : StartupGameplayEffects)
		{
			if (!IsValid(GEClass)) continue;
			// class default object
			UGameplayEffect* EffectCDO = GEClass->GetDefaultObject<UGameplayEffect>();
			InASC->ApplyGameplayEffectToSelf(EffectCDO, Level, InASC->MakeEffectContext());
		}

		UE_LOG(LogTemp, Warning, TEXT("Startup Attributes Initialized for %s."), *InASC->GetName());
	}
}

void UDataAsset_StartupDataBase::GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities, URogAbilitySystemComponent* InASC, int32 Level) const
{
	if (InAbilities.IsEmpty()) return;

	// TODO: change UGameplayAbility to my custom GameplayAbilityBase
	for (const TSubclassOf<UGameplayAbility>& Ability : InAbilities)
	{
		if (!IsValid(Ability)) continue;

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = Level;
		InASC->GiveAbility(AbilitySpec);
	}
}
