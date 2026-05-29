// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogAttributeSet.h"

#include "Net/UnrealNetwork.h"
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

	// check if the modified attribute is DamageTaken
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float LocalDamageTaken = GetDamageTaken();
		SetDamageTaken(0);

		if (LocalDamageTaken > 0.f)
		{
			const float NewHealth = FMath::Clamp(GetHealth() - LocalDamageTaken, 0.f, GetMaxHealth());
			SetHealth(NewHealth);

			const FString DebugMsg = FString::Printf(TEXT("%s received Damage: %f CurrentHealth: %f"), *Data.Target.GetAvatarActor()->GetName(), LocalDamageTaken, GetHealth());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMsg);

			// TODO: Handle Death
			if (NewHealth <= 0.0f)
			{
				// TODO: Trigger character death / ragdoll logic
				UE_LOG(LogTemp, Warning, TEXT("Character %s has died!"), *Data.Target.GetAvatarActor()->GetName());
			}
		}
	}
}

#pragma region AttributesRepnotify

void URogAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageTaken, COND_None, REPNOTIFY_Always);
}

void URogAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}

void URogAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);
}

void URogAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AttackPower, OldValue);
}

void URogAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageTaken, OldValue);
}

#pragma endregion AttributesRepnotify