// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTT_SmoothRotate.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

URogBTT_SmoothRotate::URogBTT_SmoothRotate()
{
	NodeName = "Smoothly Rotate To Target";
	bNotifyTick = true; // IMPORTANT: Enables TickTask
	RotationInterpSpeed = 5.0f;
	AcceptanceRadius = 2.0f;
}

EBTNodeResult::Type URogBTT_SmoothRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	CachedTargetActor = TargetActor;
	// Return InProgress so the Behavior Tree knows to start calling TickTask
	return EBTNodeResult::InProgress;
}

void URogBTT_SmoothRotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CachedTargetActor.IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(OwnerPawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Calculate direction and zero out Z to prevent the AI from tilting up/down
	FVector Direction = CachedTargetActor->GetActorLocation() - OwnerPawn->GetActorLocation();
	Direction.Z = 0.f;

	const FRotator TargetRotation = Direction.Rotation();
	const FRotator CurrentRotation = OwnerPawn->GetActorRotation();

	// Smoothly interpolate rotation
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationInterpSpeed);
	OwnerPawn->SetActorRotation(NewRotation);

	// Check if we are close enough to finish the task
	if (FMath::Abs(TargetRotation.Yaw - CurrentRotation.Yaw) <= AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
