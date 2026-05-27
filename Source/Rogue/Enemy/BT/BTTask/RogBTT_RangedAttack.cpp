// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTT_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/RogEnemyCharacter.h"
#include "Projectiles/RogueProjectileBase.h"

EBTNodeResult::Type URogBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARogEnemyCharacter* Character = Cast<ARogEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Character)) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;
	CachedCharacter = Character;

	if (!AttackMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("BTT_RangedAttack: AttackMontage is NULL!"));
		return EBTNodeResult::Failed;
	}

	Character->PlayAnimMontage(AttackMontage);
	Character->OnRangedAttackEvent.RemoveAll(this);
	Character->OnRangedAttackEvent.AddDynamic(this, &ThisClass::OnGameplayEventReceived);

	return EBTNodeResult::InProgress;
}

void URogBTT_RangedAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// Cleanup: unbind the delegate
	if (ARogEnemyCharacter* Character = Cast<ARogEnemyCharacter>(CachedCharacter.Get()))
	{
		Character->OnRangedAttackEvent.RemoveAll(this);

		// If the task is being aborted (e.g., another task takes over), stop the montage
		if (TaskResult == EBTNodeResult::Aborted && AttackMontage)
		{
			Character->StopAnimMontage(AttackMontage);
		}
	}

	CachedOwnerComp = nullptr;
	CachedCharacter = nullptr;
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}


void URogBTT_RangedAttack::OnGameplayEventReceived()
{
	if (!CachedOwnerComp.IsValid() || !CachedCharacter.IsValid())
	{
		return;
	}

	UBehaviorTreeComponent* OwnerComp = CachedOwnerComp.Get();
	ACharacter* Character = CachedCharacter.Get();

	// --- Spawn the projectile (same logic as before) ---
	UBlackboardComponent* BBComp = OwnerComp->GetAIOwner()->GetBlackboardComponent();
	if (!BBComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FName SocketName = BBComp->GetValueAsName(SocketNameKey.SelectedKeyName);
	const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(SocketName);
	const AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// const FVector Direction = TargetActor->GetActorLocation() - SpawnLocation;
	// FRotator SpawnRotation = Direction.Rotation();
	FRotator SpawnRotation = Character->GetActorForwardVector().Rotation();

	SpawnRotation.Pitch += FMath::FRandRange(0.f, MaxBulletSpread);
	SpawnRotation.Yaw += FMath::FRandRange(-MaxBulletSpread, MaxBulletSpread);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedProjectile = Character->GetWorld()->SpawnActor<AActor>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedProjectile)
	{
		Character->MoveIgnoreActorAdd(SpawnedProjectile);
	}

	// --- Task is done! ---
	FinishLatentTask(*OwnerComp, SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}
