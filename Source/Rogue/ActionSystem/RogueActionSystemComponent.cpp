// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool URogueActionSystemComponent::ApplyHealthChange(float InValue)
{
	const float OldHealth = Attributes.Health;
	if (FMath::IsNearlyZero(OldHealth)) return false;

	// const float MaxHealth = GetDefault<URogueActionSystemComponent>()->Attributes.Health;
	Attributes.Health = FMath::Clamp(Attributes.Health+InValue, 0.f, Attributes.HealthMax);
	OnHealthChanged.Broadcast(Attributes.Health, OldHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health changed: %f | MaxHealth: %f"), Attributes.Health, Attributes.HealthMax);
	return  true;
}
