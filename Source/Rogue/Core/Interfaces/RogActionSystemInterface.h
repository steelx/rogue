// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RogActionSystemInterface.generated.h"

class URogueActionSystemComponent;
// This class does not need to be modified.
UINTERFACE()
class URogActionSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUE_API IRogActionSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual URogueActionSystemComponent* GetActionSystemComponent() const = 0;
};
