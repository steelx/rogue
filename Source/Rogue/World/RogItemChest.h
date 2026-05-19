// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rogue/Interfaces/RogInteractableInterface.h"
#include "RogItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ROGUE_API ARogItemChest : public AActor, public IRogInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogItemChest();

	virtual void Tick(float DeltaSeconds) override;
	virtual void Interact() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Chest")
	float RotationPitch {120.f};

	UPROPERTY(EditDefaultsOnly, Category="Rogue|Chest")
	float RotationSpeed {40.f};

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> BaseMeshComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> LidMeshComp;

private:
	float CurrentPitch = 0;
};
