// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RoguePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RogInteractionComponent.h"


ARoguePlayerController::ARoguePlayerController()
{
	InteractionComponent = CreateDefaultSubobject<URogInteractionComponent>(TEXT("InteractionComponent"));
}

void ARoguePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}

	// Input Action Mapping
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARoguePlayerController::HandleInteractAction);
}

void ARoguePlayerController::HandleInteractAction()
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact();
	}
}
