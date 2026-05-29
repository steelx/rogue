// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RoguePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RogInteractionComponent.h"
#include "RoguePlayerCharacter.h"
#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "Core/MyTags.h"
#include "Core/Components/RogInputComponent.h"


ARoguePlayerController::ARoguePlayerController()
{
	InteractionComponent = CreateDefaultSubobject<URogInteractionComponent>(TEXT("InteractionComponent"));
}

void ARoguePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	ensureMsgf(InputConfigDataAsset, TEXT("InputConfigDataAsset must be assigned!"));

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(InputSubsystem)) return;

	// for (UInputMappingContext* Context : DefaultMappingContexts)
	// {
	// 	InputSubsystem->AddMappingContext(Context, 0);
	// }

	// priority zero, so we can add other contexts later with higher priority
	InputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// Set up action bindings
	URogInputComponent* RogInputComponent = Cast<URogInputComponent>(InputComponent);
	if (!IsValid(RogInputComponent))
	{
		// This needs to be set from Project Settings > Engine > Input > Default Input Component Class : RogInputComponent
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
		return;
	}

	// Input Action Mapping for Blueprint Actions
	RogInputComponent->BindNativeInputAction(InputConfigDataAsset, MyTags::InputTag_Interact, ETriggerEvent::Triggered, this, &ARoguePlayerController::HandleInteractAction);

	// Bind Ability Input Tags Activation - DataAssets HeroInputAbilities Actions
	RogInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Handle_AbilityInputPressed, &ThisClass::Handle_AbilityInputReleased);
}

void ARoguePlayerController::HandleInteractAction()
{
	if (GetPawn()->InputEnabled())
	{
		InteractionComponent->Interact();
	}
}

void ARoguePlayerController::Handle_AbilityInputPressed(FGameplayTag InputTag)
{
	const ARoguePlayerCharacter* RoguePlayerCharacter = Cast<ARoguePlayerCharacter>(GetPawn());
	URogAbilitySystemComponent* ASC = Cast<URogAbilitySystemComponent>(RoguePlayerCharacter->GetRogAbilitySystemComponent());
	ASC->OnAbilityInputPressed(InputTag);
}

void ARoguePlayerController::Handle_AbilityInputReleased(FGameplayTag InputTag)
{
	const ARoguePlayerCharacter* RoguePlayerCharacter = Cast<ARoguePlayerCharacter>(GetPawn());
	URogAbilitySystemComponent* ASC = Cast<URogAbilitySystemComponent>(RoguePlayerCharacter->GetRogAbilitySystemComponent());
	ASC->OnAbilityInputReleased(InputTag);
}
