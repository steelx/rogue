// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartupDataBase.generated.h"

class URogAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class ROGUE_API UDataAsset_StartupDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	/** GiveToAbilitySystemComponent simply grants Ability to be used, activation happens at MyAbilitySystemComponent **/
	virtual void GiveToAbilitySystemComponent(URogAbilitySystemComponent* InASC, int32 Level = 1) const;

protected:
	// Startup Abilities to be given to Character on Posses (TODO: use MyGameplayAbilityBase)
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UGameplayAbility>> ActivateOnGivenAbilities;

	// e.g. HitReact, Death, --> OnTriggered Abilities
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities, URogAbilitySystemComponent* InASC, int32 Level = 1) const;
};
