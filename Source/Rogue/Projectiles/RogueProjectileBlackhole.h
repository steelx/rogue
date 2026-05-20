// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileBlackhole.generated.h"

UCLASS()
class ROGUE_API ARogueProjectileBlackhole : public ARogueProjectileBase
{
	GENERATED_BODY()

public:
	ARogueProjectileBlackhole();

protected:
	UPROPERTY(VisibleAnywhere, Category="Projectile|Components")
	TObjectPtr<URadialForceComponent> RadialForceComp;

	virtual void BeginPlay() override;
	virtual void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	FTimerHandle ElapsedTimerHandle;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ExplosionNiagaraComp;

	UFUNCTION()
	void ExplosionSystemFinished(UNiagaraComponent* PSystem);

	void ElapsedTimerFunction();
};
