// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/RogBaseCharacter.h"
#include "RoguePlayerCharacter.generated.h"

class UCustomMovementComponent;
class UDataAsset_StartupDataBase;
class URogAbilitySystemComponent;
class URogAttributeSet;
class ARogueProjectileBase;
class ARogueProjectileBlackhole;
class UNiagaraSystem;
class ARogProjectileMagic;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ROGUE_API ARoguePlayerCharacter : public ARogBaseCharacter
{
	GENERATED_BODY()

public:
	ARoguePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	URogAbilitySystemComponent* GetRogAbilitySystemComponent() const;
	URogAttributeSet* GetAttributeSet() const;
protected:

	// Assign PlayerStartupDataBase which will call prototype DataAsset_PlayerStartupData::GiveToAbilitySystemComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilitySystem|CharacterData")
	TSoftObjectPtr<UDataAsset_StartupDataBase> StartupData;

	/// Primary Attack
	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	FName PrimaryAttackSocket {"Muzzle_01"};

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TSubclassOf<ARogProjectileMagic> ProjectileMagicClass;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TObjectPtr<UNiagaraSystem> PrimaryAttackCastingEffect;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TObjectPtr<USoundBase> PrimaryAttackCastingSound;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Animation")
	TObjectPtr<UAnimMontage> PrimaryAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Animation")
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	/// Super Attack
	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TSubclassOf<ARogueProjectileBlackhole> ProjectileBlackholeClass;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TObjectPtr<UNiagaraSystem> SuperAttackCastingEffect;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TObjectPtr<USoundBase> SuperAttackCastingSound;

	/// Teleport Attack
	UPROPERTY(EditDefaultsOnly, Category="Rogue|Projectile")
	TSubclassOf<ARogueProjectileBase> ProjectileTeleportClass;

	/// -- Components --
#pragma region Components
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCustomMovementComponent> CustomMovementComponent;
#pragma endregion  Components

#pragma region InputActions
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> PrimaryAttackAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> SuperAttackAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> TeleportAction;
#pragma endregion InputActions


private:
	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);
	void JumpInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	void HandlePrimaryAttack();
	void HandleSuperAttack();
	void HandleTeleportAction();

	void PrimaryAttackTimerElapsed();
	void SuperAttackTimerElapsed();
	void TeleportActionTimerElapsed();
};
