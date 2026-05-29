// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogEnemyCharacter.h"

#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "AbilitySystem/RogAttributeSet.h"
#include "DataAssets/StartupData/DataAsset_StartupDataBase.h"
#include "Engine/AssetManager.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


ARogEnemyCharacter::ARogEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<URogAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<URogAttributeSet>(TEXT("AttributeSet"));
}

void ARogEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent())) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	ensureMsgf(!StartupData.IsNull(), TEXT("StartupData is null in %s. Please assign a valid DataAsset_StartupDataBase."), *GetName());

	// Bind Delegates on BOTH Server and Client
	if (const URogAttributeSet* Rog_AttributeSet = GetAttributeSet())
	{
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Rog_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
	}

	/// Load and Apply Startup Data ONLY on the Server
	if (HasAuthority() && !StartupData.IsNull())
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartupData.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[this]()
				{
					if (UDataAsset_StartupDataBase* LoadedData = StartupData.Get())
					{
						LoadedData->GiveToAbilitySystemComponent(GetRogAbilitySystemComponent());
					}
				}
			)
		);
	}
}

UAbilitySystemComponent* ARogEnemyCharacter::GetAbilitySystemComponent() const
{
	return GetRogAbilitySystemComponent();
}

void ARogEnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched)
}

URogAbilitySystemComponent* ARogEnemyCharacter::GetRogAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URogAttributeSet* ARogEnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}
