// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/RogActionSystemInterface.h"
#include "RogBaseCharacter.generated.h"

class URogueActionSystemComponent;
class AController;

UCLASS()
class ROGUE_API ARogBaseCharacter : public ACharacter, public IRogActionSystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogBaseCharacter();

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//~Begin IRogActionSystemInterface
	UFUNCTION(BlueprintCallable)
	virtual URogueActionSystemComponent* GetActionSystemComponent() const override { return ActionSystemComponent; }
	//~End IRogActionSystemInterface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Rogue|ActionSystem")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;

	UFUNCTION()
	virtual void HandleHealthChanged(float NewHealth, float OldHealth);
};
