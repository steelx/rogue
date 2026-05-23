#include "STTask_LookInRandomDirection.h"

#include "CubeActor.h"
#include "StateTreeExecutionContext.h"
#include "Kismet/KismetMathLibrary.h"

EStateTreeRunStatus FSTTask_LookInRandomDirection::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.OwningActor) return EStateTreeRunStatus::Failed;

	InstanceData.OwningActor->LookAt(UKismetMathLibrary::RandomRotator());

	return EStateTreeRunStatus::Running;
}
