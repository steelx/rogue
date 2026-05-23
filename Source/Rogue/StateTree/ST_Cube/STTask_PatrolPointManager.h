// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_PatrolPointManager.generated.h"

class ARogPatrolPointManager;

USTRUCT()
struct FST_PatrolPointInstanceData
{
	GENERATED_BODY()

	/// OUTPUT
	UPROPERTY(VisibleAnywhere, Category="Output")
	TObjectPtr<ARogPatrolPointManager> PatrolPointManager;
};

USTRUCT()
struct FSTTask_PatrolPointManager: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FST_PatrolPointInstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FST_PatrolPointInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
