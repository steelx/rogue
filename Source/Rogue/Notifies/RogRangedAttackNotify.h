// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RogRangedAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_API URogRangedAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	// FGameplayTag EventToSend {MyTags::Event_Owner_Attack_Ranged};
};
