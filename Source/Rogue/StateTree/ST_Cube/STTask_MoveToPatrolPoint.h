// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_MoveToPatrolPoint.generated.h"

class ACubeActor;
class ARogPatrolPointManager;

USTRUCT()
struct FST_MoveToPatrolPointInstanceData
{
	GENERATED_BODY()

	/// CONTEXT
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACubeActor> OwningActor;

	/// INPUT
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<ARogPatrolPointManager> PatrolPointManager;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MoveSpeed {1.f};

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float AcceptanceRadius {10.f};

	/// INTERNAL / OUTPUT - stores the target location for movement
	UPROPERTY()
	FVector PatrolPointLocation = FVector::ZeroVector;
};


USTRUCT()
struct FSTTask_MoveToPatrolPoint: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FST_MoveToPatrolPointInstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FST_MoveToPatrolPointInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
