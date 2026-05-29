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

	/**
	 * Creates a dynamically evaluated Gameplay Effect Spec for dealing damage.
	 * @param EffectClass           The Gameplay Effect class to instantiate (e.g., GE_Enemy_Projectile_Damage).
	 * @param InBaseDamage            The raw base damage value to apply (injected via SetByCaller if applicable).
	 * @param InCurrentAttackTypeTag  Optional; If NO tag is passed, the Spec relies on the GE AttackPower Curve Table
	 * @param InCurrentComboCount     The multiplier or magnitude to associate with the attack tag (defaults to 1).
	 * @return                        A finalized Gameplay Effect Spec Handle ready to be applied to a target.
	 */
	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakeDamageEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, float InBaseDamage, FGameplayTag InCurrentAttackTypeTag = FGameplayTag(), int32 InCurrentComboCount = 1) const;
};
