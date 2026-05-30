// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartupDataBase.h"
#include "DataAsset_PlayerStartupData.generated.h"

struct FInputAbilitySet;
/**
 * 
 */
UCLASS()
class ROGUE_API UDataAsset_PlayerStartupData : public UDataAsset_StartupDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(URogAbilitySystemComponent* InASC, int32 Level = 1) const override;

protected:
	// Bind Input tag to existing Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "StartupData", meta = (TitleProperty = "InputTag", Categories="MyTags.InputTag"))
	TArray<FInputAbilitySet> HeroInputAbilities;

};
