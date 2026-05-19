// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogueGameMode.h"

#include "Player/RoguePlayerController.h"

ARogueGameMode::ARogueGameMode()
{
	PlayerControllerClass = ARoguePlayerController::StaticClass();
}
