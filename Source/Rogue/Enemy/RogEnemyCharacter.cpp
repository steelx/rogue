// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogEnemyCharacter.h"


ARogEnemyCharacter::ARogEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARogEnemyCharacter::HandleHealthChanged(float NewHealth, float OldHealth)
{
	Super::HandleHealthChanged(NewHealth, OldHealth);

	UE_LOG(LogTemp, Verbose, TEXT("ARogEnemyCharacter Health changed!"));
}
