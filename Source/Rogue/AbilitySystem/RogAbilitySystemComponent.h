// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RogAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API URogAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URogAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, float InBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount) const;
};
