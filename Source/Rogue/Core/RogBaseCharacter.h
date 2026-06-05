// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "RogBaseCharacter.generated.h"

struct FOnAttributeChangeData;
class UGameplayEffect;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, Attributes);

UCLASS()
class ROGUE_API ARogBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogBaseCharacter(const FObjectInitializer& ObjectInitializer);

	//~Begin IAbilitySystemInterface
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End IAbilitySystemInterface

	bool IsAlive() const { return bAlive; }
	void SetAlive(bool bAliveStatus) { bAlive = bAliveStatus; }
	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;

	UFUNCTION(BlueprintCallable, Category = "Crash|Death")
	virtual void HandleRespawn();

	UFUNCTION(BlueprintCallable, Category = "Crash|Attributes")
	void ResetAttributes();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;

	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	bool bAlive = true;
};
