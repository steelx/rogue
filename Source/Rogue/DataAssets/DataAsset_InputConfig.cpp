// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FRogInputActionConfig& Config : NativeInputActions)
	{
		if (Config.InputTag == InInputTag && Config.InputAction)
		{
			return Config.InputAction;
		}
	}

	return nullptr;
}
