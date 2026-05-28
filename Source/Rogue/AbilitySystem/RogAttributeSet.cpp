// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogAttributeSet.h"

#include "GameplayEffectExtension.h"

URogAttributeSet::URogAttributeSet()
{
	// Startup Data will load Gameplay Effect to populate below values correctly
	// GE Instant _> Gameplay Effect > Modifiers (Override) -> set value OR use Linear Curve Table
	InitMaxHealth(1.f);
	InitHealth(1.f);
	InitAttackPower(1.f);
	InitDamageTaken(0);
}

void URogAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Gameplay Effect (like a potion) directly modified the Health attribute
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp the permanent base value of Health
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		const FString DebugMsg = FString::Printf(TEXT("Healing received: New Health %f"), GetHealth());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMsg);
	}

	// check if the modified attribute is DamageTaken
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float LocalDamageTaken = GetDamageTaken();
		SetDamageTaken(0);

		const FString DebugMsg = FString::Printf(TEXT("Damage received: %f Health before DamageDone: %f"), LocalDamageTaken, GetHealth());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMsg);

		if (LocalDamageTaken > 0.f)
		{
			const float NewHealth = FMath::Clamp(GetHealth() - LocalDamageTaken, 0.f, GetMaxHealth());
			SetHealth(NewHealth);

			UE_LOG(LogTemp, Log, TEXT("Damage applied: %f. New Health: %f"), LocalDamageTaken, NewHealth);

			// TODO: Handle Death
			if (NewHealth <= 0.0f)
			{
				// TODO: Trigger character death / ragdoll logic
			}
		}
	}
}
