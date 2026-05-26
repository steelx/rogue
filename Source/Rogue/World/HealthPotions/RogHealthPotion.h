// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/RogPickupActor.h"
#include "RogHealthPotion.generated.h"

class UNiagaraSystem;
class UStaticMeshComponent;
class USphereComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class ROGUE_API ARogHealthPotion : public ARogPickupActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogHealthPotion();
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthPotion|Healing")
	float HealAmount {100.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthPotion|Healing")
	TObjectPtr<UAudioComponent> LoopingAudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthPotion|Healing")
	TObjectPtr<USoundBase> HealingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthPotion|Healing")
	TObjectPtr<UNiagaraSystem> HealingEffect;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HealthPotion")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HealthPotion")
	TObjectPtr<UTimelineComponent> MeshAnimTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthPotion")
	TObjectPtr<UCurveFloat> MeshAnimCurveFloat;

	virtual void OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UFUNCTION()
	void MeshAnimTimelineFinished();

	UFUNCTION()
	void MeshAnimTimelineUpdate(float Alpha);
};
