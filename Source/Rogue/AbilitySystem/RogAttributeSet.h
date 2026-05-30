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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, AttackPower);

	// used as a placeholder for Damage calculations
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DamageTaken)
	FGameplayAttributeData DamageTaken;
	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, DamageTaken);

};
