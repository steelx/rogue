// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "DataAsset_PlayerStartupData.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "Core/RogStructTypes.h"

void UDataAsset_PlayerStartupData::GiveToAbilitySystemComponent(URogAbilitySystemComponent* InASC, int32 Level) const
{
	Super::GiveToAbilitySystemComponent(InASC, Level);

	for (const FInputAbilitySet& AbilitySet : HeroInputAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.Level = Level;
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		InASC->GiveAbility(AbilitySpec);
	}
}
