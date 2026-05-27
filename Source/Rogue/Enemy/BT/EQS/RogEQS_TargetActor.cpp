// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogEQS_TargetActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/RogueGameTypes.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void URogEQS_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if (!QuerierObject) return;

	const APawn* QuerierPawn = Cast<APawn>(QuerierObject);
	if (!QuerierPawn) return;

	AAIController* Controller = Cast<AAIController>(QuerierPawn->GetController());

	if (!IsValid(Controller))
	{
		// we need this for EQS testing pawn
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, QuerierPawn);
		return;
	}

	const UBlackboardComponent* BlackboardComponent = Controller->GetBlackboardComponent();
	if (!IsValid(BlackboardComponent))
	{
		// we need this for EQS testing pawn
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, QuerierPawn);
		return;
	}

	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(NAME_TargetActor));
	if (!IsValid(TargetActor)) return;

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
}
