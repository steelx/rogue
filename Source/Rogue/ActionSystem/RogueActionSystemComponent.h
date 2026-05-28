// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthAttributesChangedSignature, float, NewHealth, float, OldHealth);

USTRUCT(BlueprintType)
struct FRogueAttributesSet
{
	GENERATED_BODY()

	FRogueAttributesSet(): HealthMax(100.f), Health(100.f) {}

	UPROPERTY(BlueprintReadOnly)
	float HealthMax;

	UPROPERTY(BlueprintReadOnly)
	float Health;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URogueActionSystemComponent();

	bool ApplyHealthChange(float InValue);

	UPROPERTY(BlueprintAssignable)
	FOnHealthAttributesChangedSignature OnHealthChanged;

	FRogueAttributesSet GetAttributesSet() const { return Attributes; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ActionSystem|Attributes")
	FRogueAttributesSet Attributes;
};
