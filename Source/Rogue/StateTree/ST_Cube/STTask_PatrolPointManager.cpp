#include "STTask_PatrolPointManager.h"

#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"
#include "StateTree/RogPatrolPointManager.h"

EStateTreeRunStatus FSTTask_PatrolPointManager::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const UWorld* World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("FSTTask_PatrolPointManager : Failed to GetWorld context!"));
		return EStateTreeRunStatus::Failed;
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(World, ARogPatrolPointManager::StaticClass(), OutActors);

	if (OutActors.Num() > 0)
	{
		if (ARogPatrolPointManager* Manager = Cast<ARogPatrolPointManager>(OutActors[0]))
		{
			InstanceData.PatrolPointManager = Manager;
			UE_LOG(LogTemp, Warning, TEXT("FSTTask_PatrolPointManager : Succeeded to Get PatrolPointManager"));

			// IMPORTANT: Return 'Running' so the Global Task stays active in the background
			// and doesn't prematurely terminate the entire State Tree.
			return EStateTreeRunStatus::Running;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("FSTTask_PatrolPointManager : Failed to Get PatrolPointManager!"));
	return EStateTreeRunStatus::Failed;
}
