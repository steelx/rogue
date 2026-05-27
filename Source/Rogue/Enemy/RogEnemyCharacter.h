// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRogOnRangedAttackEventDelegate);

UCLASS()
class ROGUE_API ARogEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogEnemyCharacter();

	/** Broadcast when the AnimNotify in the RangedAttack montage fires */
	UPROPERTY(BlueprintAssignable, Category = "Rogue|Events")
	FRogOnRangedAttackEventDelegate OnRangedAttackEvent;
};
