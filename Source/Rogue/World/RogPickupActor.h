// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogPickupActor.generated.h"

class USphereComponent;

UCLASS()
class ROGUE_API ARogPickupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogPickupActor();

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> SphereCollision;

	UFUNCTION()
	virtual void OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
