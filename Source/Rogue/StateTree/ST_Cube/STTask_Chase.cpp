#include "STTask_Chase.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTTask_Chase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!IsValid(InstanceData.OwningActor) || !IsValid(InstanceData.TargetPawn))
	{
		return EStateTreeRunStatus::Failed;
	}

	const FVector CurrentLocation = InstanceData.OwningActor->GetActorLocation();
	const FVector TargetLocation = InstanceData.TargetPawn->GetActorLocation();

	if (TargetLocation.Equals(CurrentLocation, InstanceData.InterpSpeed))
	{
		return EStateTreeRunStatus::Succeeded;
	}

	const FVector Location = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InstanceData.InterpSpeed);
	InstanceData.OwningActor->SetActorLocation(Location);

	return EStateTreeRunStatus::Running;
}
