// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class ARogueProjectileBase;
class ARogueProjectileBlackhole;
class UNiagaraSystem;
class ARogProjectileMagic;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ROGUE_API ARoguePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoguePlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

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
