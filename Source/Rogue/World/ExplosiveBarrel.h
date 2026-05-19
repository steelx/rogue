// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;
class URadialForceComponent;

UCLASS()
class ROGUE_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplosiveBarrel();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Barrel")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Barrel|Components")
	TObjectPtr<URadialForceComponent> RadialForceComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Barrel|Components")
	TObjectPtr<UAudioComponent> ActiveBurningSoundComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Components")
	TObjectPtr<UNiagaraComponent> ActiveBurningEffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|FX")
	TObjectPtr<USoundBase> ActiveBurningSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|FX")
	TObjectPtr<UNiagaraSystem> ActiveBurningEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|FX")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|FX")
	TObjectPtr<USoundBase> ExplosionSound;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|FX", meta = (AllowPrivateAccess = true))
	float ExplosionTriggerTimerDelay {3.f};

	FTimerHandle ExplosionTriggerTimerHandle;

	bool bExploded = false;
	void TriggerExplosion();
};
