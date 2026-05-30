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
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	URogAbilitySystemComponent* GetRogAbilitySystemComponent() const;
	URogAttributeSet* GetAttributeSet() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsBeingLaunched{false};

	/** Broadcast when the AnimNotify in the RangedAttack montage fires */
	UPROPERTY(BlueprintAssignable, Category = "Rogue|Events")
	FRogOnRangedAttackEventDelegate OnRangedAttackEvent;

protected:
#pragma region AbilitySystem
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|AbilitySystem")
	TObjectPtr<URogAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<URogAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rogue|AbilitySystem")
	TSoftObjectPtr<UDataAsset_StartupDataBase> StartupData;
#pragma endregion AbilitySystem

};
