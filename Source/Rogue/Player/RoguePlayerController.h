// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguePlayerController.generated.h"

class URogInteractionComponent;
class UInputMappingContext;
/**
 *
 */
UCLASS()
class ROGUE_API ARoguePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARoguePlayerController();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Rogue|Components")
	TObjectPtr<URogInteractionComponent> InteractionComponent;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Rogue|Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	virtual void SetupInputComponent() override;
};
