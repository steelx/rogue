// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_Chase.generated.h"


USTRUCT()
struct FST_ChaseInstanceData
{
	GENERATED_BODY()

	/// CONTEXT
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> OwningActor;

	/// INPUT
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<APawn> TargetPawn;

	/// PARAMETER
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float InterpSpeed {2.f};
};

USTRUCT(meta = (DisplayName = "Chase Target Pawn"))
struct FSTTask_Chase: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FST_ChaseInstanceData;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FST_ChaseInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
