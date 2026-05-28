// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interfaces/RogInteractableInterface.h"
#include "GameFramework/Actor.h"
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

	//~Begin IRogInteractableInterface
	virtual void Interact_Implementation() override;
	//~End IRogInteractableInterface
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="InteractionFinished"))
	void BP_InteractionFinished();

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
