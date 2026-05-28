// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RogPlayerState.generated.h"

class URogAttributeSet;
class URogAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ROGUE_API ARogPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARogPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE URogAbilitySystemComponent* GetRogAbilitySystemComponent() const {return AbilitySystemComponent; }
	FORCEINLINE URogAttributeSet* GetAttributeSet() const {return AttributeSet;}

private:
	UPROPERTY(VisibleAnywhere, Category="Rogue|AbilitySystem")
	TObjectPtr<URogAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category="Rogue|AbilitySystem")
	TObjectPtr<URogAttributeSet> AttributeSet;
};
