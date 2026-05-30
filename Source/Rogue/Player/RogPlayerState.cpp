// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogPlayerState.h"

#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "AbilitySystem/RogAttributeSet.h"

ARogPlayerState::ARogPlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<URogAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<URogAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ARogPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
