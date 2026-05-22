// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "STTask_RotateCube.h"

#include "StateTreeExecutionContext.h"
#include "GameFramework/Actor.h"

EStateTreeRunStatus FSTTask_RotateCube::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.OwningActor)
	{
		return EStateTreeRunStatus::Failed;
	}

	// Apply relative rotation based on RotatingRate (degrees per second)
	const FRotator DeltaRotation(0, InstanceData.RotatingRate*DeltaTime, 0);
	InstanceData.OwningActor->AddActorLocalRotation(DeltaRotation);

	InstanceData.RotatingTimer -= DeltaTime;
	if (InstanceData.RotatingTimer <= 0.f)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}
