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

			FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
			ContextHandle.AddInstigator(InASC->GetAvatarActor(), InASC->GetAvatarActor());
			FGameplayEffectSpecHandle EffectSpecHandle = InASC->MakeOutgoingSpec(GEClass, Level, ContextHandle);
			InASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			// Retrieve the CDO purely for iterating the base Modifiers for our Debug Log
			const UGameplayEffect* EffectCDO = GEClass->GetDefaultObject<UGameplayEffect>();
			FString DebugString = FString::Printf(
		TEXT("Applied GE [%s] -> [%s]\n"), *GEClass->GetName(), *GetNameSafe(InASC->GetAvatarActor()));
			for (const FGameplayModifierInfo& Modifier : EffectCDO->Modifiers)
			{
				DebugString += FString::Printf(
					TEXT("  - %s (%s)\n"),
					*Modifier.Attribute.AttributeName,
					*StaticEnum<EGameplayModOp::Type>()->GetNameStringByValue(Modifier.ModifierOp));
			}
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugString);
		}
	}
}

void UDataAsset_StartupDataBase::GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities, URogAbilitySystemComponent* InASC, int32 Level) const
{
	if (InAbilities.IsEmpty()) return;

	for (const TSubclassOf<UGameplayAbility>& Ability : InAbilities)
	{
		if (!IsValid(Ability)) continue;

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = Level;
		InASC->GiveAbility(AbilitySpec);
	}
}
