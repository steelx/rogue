// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileTeleport.generated.h"

UCLASS()
class ROGUE_API ARogueProjectileTeleport : public ARogueProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogueProjectileTeleport();

	virtual void PostInitializeComponents() override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float TravelTimerDelay {3.0f};

	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	FTimerHandle TravelTimerHandle;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ExplosionNiagaraComp;

	UFUNCTION()
	void TravelTimerElapsed();

	UFUNCTION()
	void ExplosionSystemFinished(UNiagaraComponent* PSystem);
};
