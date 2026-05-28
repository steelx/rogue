// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogEnemyCharacter.h"

#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "DataAssets/StartupData/DataAsset_StartupDataBase.h"
#include "Engine/AssetManager.h"


ARogEnemyCharacter::ARogEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<URogAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ARogEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsValid(GetAbilitySystemComponent())) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	ensureMsgf(!StartupData.IsNull(), TEXT("StartupData is null in %s. Please assign a valid DataAsset_StartupDataBase."), *GetName());

	if (StartupData.IsNull()) return;

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

UAbilitySystemComponent* ARogEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
