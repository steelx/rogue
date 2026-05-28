// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBaseCharacter.h"

#include "ActionSystem/RogueActionSystemComponent.h"


// Sets default values
ARogBaseCharacter::ARogBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComponent"));
}

void ARogBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARogBaseCharacter::HandleHealthChanged);
}

float ARogBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ActionSystemComponent->ApplyHealthChange(-DamageTaken);

	return DamageTaken;
}

void ARogBaseCharacter::HandleHealthChanged(float NewHealth, float OldHealth)
{
	UE_LOG(LogTemp, Verbose, TEXT("ARogBaseCharacter Health changed: %f"), NewHealth);
}
