// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RogBaseCharacter.h"
#include "RogEnemyCharacter.generated.h"

class URogAttributeSet;
class UDataAsset_StartupDataBase;
class URogAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRogOnRangedAttackEventDelegate);

UCLASS()
class ROGUE_API ARogEnemyCharacter : public ARogBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogEnemyCharacter();
	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE URogAbilitySystemComponent* GetRogAbilitySystemComponent() const { return AbilitySystemComponent; }
	FORCEINLINE URogAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Broadcast when the AnimNotify in the RangedAttack montage fires */
	UPROPERTY(BlueprintAssignable, Category = "Rogue|Events")
	FRogOnRangedAttackEventDelegate OnRangedAttackEvent;

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|AbilitySystem")
	TObjectPtr<URogAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<URogAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystem|CharacterData")
	TSoftObjectPtr<UDataAsset_StartupDataBase> StartupData;
#pragma endregion Components

};
