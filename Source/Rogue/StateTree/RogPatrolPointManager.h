// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogPatrolPointManager.generated.h"

class ATargetPoint;

UCLASS()
class ROGUE_API ARogPatrolPointManager : public AActor
{
	GENERATED_BODY()

public:
	ARogPatrolPointManager();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	ATargetPoint* GetNextPatrolPoint();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Manager|Components")
	TObjectPtr<UBillboardComponent> BillboardIcon;

	UPROPERTY(EditInstanceOnly, Category = "Manager|Components")
	TArray<ATargetPoint*> PatrolPoints;

private:
	int8 CurrentPatrolPointIndex = INDEX_NONE;
};

