#pragma once

#include "GameplayTagContainer.h"

#include "RogStructTypes.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FInputAbilitySet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Category="InputTag", Categories="MyTags.InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText InputKeyName;// InputTag -> Keyboard key name

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

