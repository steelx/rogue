// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "RogAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Managed automatically by UE GAS whenever ability is applied
 * Example: we create GE_Player_Startup gameplay effect to initialize attributes on character spawn
 * such as Health, MaxHealth, etc. Which are applied from DataAsset_StartupData to AbilitySystemComponent
 */
UCLASS()
class ROGUE_API URogAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URogAttributeSet();
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(ThisClass, AttackPower);

	// used as a placeholder for Damage calculations
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(ThisClass, DamageTaken);

};
