// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RogBaseCharacter.h"
#include "RogEnemyCharacter.generated.h"

class URogueActionSystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRogOnRangedAttackEventDelegate);

UCLASS()
class ROGUE_API ARogEnemyCharacter : public ARogBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogEnemyCharacter();

	/** Broadcast when the AnimNotify in the RangedAttack montage fires */
	UPROPERTY(BlueprintAssignable, Category = "Rogue|Events")
	FRogOnRangedAttackEventDelegate OnRangedAttackEvent;

protected:
	virtual void HandleHealthChanged(float NewHealth, float OldHealth) override;
};
