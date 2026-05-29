// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"

#include "RogInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API URogInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InputConfig, const FGameplayTag& InInputAction, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};


template <class UserObject, typename CallbackFunc>
void URogInputComponent::BindNativeInputAction(
	const UDataAsset_InputConfig* InputConfig,
	const FGameplayTag& InInputAction,
	ETriggerEvent TriggerEvent,
	UserObject* ContextObject,
	CallbackFunc Func
)
{
	checkf(InputConfig, TEXT("InputConfig is null in BindNativeInputAction"));

	UInputAction* FoundAction = InputConfig->FindNativeInputActionByTag(InInputAction);
	if (!FoundAction) return;

	this->BindAction(FoundAction, TriggerEvent, ContextObject, Func);
}

template <class UserObject, typename CallbackFunc>
void URogInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InputConfig, TEXT("InputConfig is null in BindAbilityInputAction"));

	for (const FRogInputActionConfig& AbilityInputActionConfig : InputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid()) continue;
		UInputAction* FoundAction = AbilityInputActionConfig.InputAction;

		this->BindAction(FoundAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		this->BindAction(FoundAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
