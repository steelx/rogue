// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API URogInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URogInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float InteractionRadius {400.f};

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	bool bDrawDebugs {false};

private:
	TSoftObjectPtr<AActor> SelectedActor;
};
