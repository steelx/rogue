// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_RotateCube.generated.h"

USTRUCT()
struct FST_RotateCubeInstanceData
{
	GENERATED_BODY()

	/// CONTEXT
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> OwningActor;

	/// INPUT
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<AActor> TargetCube;

	/// PARAMETER
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float RotatingTimer {2.0f};

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float RotatingRate {45.0f};
};

USTRUCT(meta = (DisplayName = "STT Rotate Cube"))
struct FSTTask_RotateCube : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FST_RotateCubeInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FST_RotateCubeInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
