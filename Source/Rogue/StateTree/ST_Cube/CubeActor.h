// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeActor.generated.h"

class USphereComponent;
class UStateTreeComponent;

UCLASS()
class ROGUE_API ACubeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACubeActor();
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube")
	TObjectPtr<APawn> TargetPawn;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube|Components")
	TObjectPtr<UStaticMeshComponent> CubeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cube|Components")
	TObjectPtr<UStateTreeComponent> StateTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube|Components")
	TObjectPtr<USphereComponent> SphereCollision;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
};
