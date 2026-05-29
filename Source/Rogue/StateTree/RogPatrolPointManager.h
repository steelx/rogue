// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogPatrolPointManager.generated.h"

class ATargetPoint;
class UBillboardComponent;

UCLASS()
class ROGUE_API ARogPatrolPointManager : public AActor
{
	GENERATED_BODY()

public:
	ARogPatrolPointManager();

	UFUNCTION(BlueprintCallable)
	ATargetPoint* GetNextPatrolPoint();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Manager|Components")
	TObjectPtr<UBillboardComponent> BillboardIcon;

	UPROPERTY(EditInstanceOnly, Category = "Manager|Components")
	TArray<ATargetPoint*> PatrolPoints;

private:
	int8 CurrentPatrolPointIndex = INDEX_NONE;
};

