// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguePlayerController.generated.h"

struct FGameplayTag;
class UDataAsset_InputConfig;
class UInputAction;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rogue|Input")
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	UPROPERTY(EditAnywhere, Category="Rogue|Input")
	TObjectPtr<UInputAction> InteractAction;

	virtual void SetupInputComponent() override;

private:
	void HandleInteractAction();

	void Handle_AbilityInputPressed(FGameplayTag InputTag);
	void Handle_AbilityInputReleased(FGameplayTag InputTag);

	void ActivateAbility(const FGameplayTag& MyTag) const;
};
