// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_LookInRandomDirection.generated.h"


class ACubeActor;

USTRUCT()
struct FST_LookInRandomDirectionInstanceData
{
	GENERATED_BODY()

	/// CONTEXT
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACubeActor> OwningActor;

};

USTRUCT()
struct FSTTask_LookInRandomDirection: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FST_LookInRandomDirectionInstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FST_LookInRandomDirectionInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
