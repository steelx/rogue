// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RogFunctionsLibrary.generated.h"


struct FGameplayTag;
struct FGameplayEffectSpecHandle;
class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class ROGUE_API URogFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Rogue|AbilitySystem")
	static FGameplayEffectSpecHandle MakeDamageEffectSpec(
		UAbilitySystemComponent* ASCWhoFired,
		TSubclassOf<UGameplayEffect> EffectClass,
		float InBaseDamage,
		FGameplayTag InCurrentAttackTypeTag,
		int32 InCurrentComboCount,
		int32 Level = 1
	);
};
