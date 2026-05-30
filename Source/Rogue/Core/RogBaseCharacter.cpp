// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARogBaseCharacter::ARogBaseCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	// Tick and refresh bone transforms whether rendered or not - for bone updates on a dedicated server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ARogBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bAlive);
}

UAbilitySystemComponent* ARogBaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void ARogBaseCharacter::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	if (AttributeChangeData.NewValue <= 0.f)
	{
		HandleDeath();
	}
}

void ARogBaseCharacter::HandleDeath()
{
	bAlive = false;
	UE_LOG(LogTemp, Warning, TEXT("RogBaseCharacter Died: %s"), *GetName());
}

void ARogBaseCharacter::HandleRespawn()
{
	bAlive = true;
}

void ARogBaseCharacter::ResetAttributes()
{
	checkf(IsValid(ResetAttributesEffect), TEXT("ResetAttributesEffect not set."));
	if (!IsValid(GetAbilitySystemComponent())) return;

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(ResetAttributesEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
