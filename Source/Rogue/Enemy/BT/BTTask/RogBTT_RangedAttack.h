// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RogBTT_RangedAttack.generated.h"

class ARogueProjectileBase;
/**
 * BT Task Ranged Attack spawns a Projectile
 */
UCLASS()
class ROGUE_API URogBTT_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	/** Called when the character's AnimNotify fires the gameplay event */
	UFUNCTION()
	void OnGameplayEventReceived();

protected:
	UPROPERTY(EditAnywhere, Category = "BTTask")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "BTTask")
	TSubclassOf<ARogueProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "BTTask")
	float MaxBulletSpread {5.0};// 5 deg spread

	UPROPERTY(EditAnywhere, Category = "BTTask")
	FBlackboardKeySelector SocketNameKey;

	UPROPERTY(EditAnywhere, Category = "BTTask")
	FBlackboardKeySelector TargetActorKey;

private:
	/** Weak reference to the BT component so we can finish the latent task later */
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	/** Cached character we're controlling */
	TWeakObjectPtr<ACharacter> CachedCharacter;
};
