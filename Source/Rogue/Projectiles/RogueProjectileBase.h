// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileBase.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;
class URadialForceComponent;
class UAudioComponent;
class USoundBase;

UCLASS(Abstract)
class ROGUE_API ARogueProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ARogueProjectileBase();

	virtual void PostInitializeComponents() override;
protected:

	UPROPERTY(VisibleAnywhere, Category="Projectile|Components")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Components")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Projectile|Effects")
	TObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY(EditAnywhere, Category="Projectile|Config")
	float ElapsedTimerDelay {5.0f};


protected:
	UFUNCTION()
	virtual void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
