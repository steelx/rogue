// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableTargetChanged, AActor*, NewTarget);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API URogInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URogInteractionComponent();
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnInteractableTargetChanged OnTargetChanged;

	// The ability calls this!
	AActor* GetCurrentTarget() const { return SelectedActor.Get(); }
	void Interact() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float InteractionRadius {400.f};

private:
	TSoftObjectPtr<AActor> SelectedActor;

	float TimerDelay {0.25f};
	FTimerHandle TimerHandle;

	UFUNCTION()
	void PerformInteractionTrace();
};
