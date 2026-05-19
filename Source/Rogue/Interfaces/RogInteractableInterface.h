// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RogInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URogInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ROGUE_API IRogInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void Interact();
};
