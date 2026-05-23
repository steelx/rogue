#include "STTask_MoveToPatrolPoint.h"

#include "CubeActor.h"
#include "StateTreeExecutionContext.h"
#include "Engine/TargetPoint.h"
#include "StateTree/RogPatrolPointManager.h"

EStateTreeRunStatus FSTTask_MoveToPatrolPoint::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.PatrolPointManager)
	{
		return EStateTreeRunStatus::Failed;
	}

	const ATargetPoint* NextPoint = InstanceData.PatrolPointManager->GetNextPatrolPoint();
	if (!NextPoint) return EStateTreeRunStatus::Failed;

	InstanceData.PatrolPointLocation = NextPoint->GetActorLocation();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_MoveToPatrolPoint::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!IsValid(InstanceData.OwningActor)) return EStateTreeRunStatus::Failed;

	const FVector CurrentLocation = InstanceData.OwningActor->GetActorLocation();
	if (FVector::Dist(CurrentLocation, InstanceData.PatrolPointLocation) <= InstanceData.AcceptanceRadius)
	{
		InstanceData.OwningActor->LookChance = FMath::RandRange(0.0f, 1.0f);
		return EStateTreeRunStatus::Succeeded;
	}

	const FVector MoveToLocation = FMath::VInterpTo(CurrentLocation, InstanceData.PatrolPointLocation, DeltaTime, InstanceData.MoveSpeed);
	InstanceData.OwningActor->SetActorLocation(MoveToLocation);

	return EStateTreeRunStatus::Running;
}
