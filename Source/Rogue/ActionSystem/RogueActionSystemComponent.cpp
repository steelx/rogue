// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URogueActionSystemComponent::ApplyHealthChange(float InValue)
{
	const float OldHealth = Attributes.Health;
	Attributes.Health += InValue;
	OnHealthChanged.Broadcast(Attributes.Health, OldHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health changed: %f"), Attributes.Health);
}
