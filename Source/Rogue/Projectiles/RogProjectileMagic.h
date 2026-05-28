// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "GameFramework/Actor.h"
#include "RogProjectileMagic.generated.h"


class UGameplayEffect;

UCLASS(Abstract)
class ROGUE_API ARogProjectileMagic : public ARogueProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogProjectileMagic();

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|GAS")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|GAS")
	float DamageAmount{ 20.f };

	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
