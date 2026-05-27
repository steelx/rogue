// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTS_CheckRangeTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void URogBTS_CheckRangeTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	const AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return;

	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	const APawn* OwningPawn = Controller->GetPawn();
	const FVector OriginLocation = OwningPawn->GetActorLocation();
	const float MaxAttackRange = BBComp->GetValueAsFloat(MaxAttackRangeKey.SelectedKeyName);
	const float DistanceTo = FVector::Dist(TargetActor->GetActorLocation(), OriginLocation);
	const bool bIsWithinRange = DistanceTo < MaxAttackRange;
	const bool bHasLOS = Controller->LineOfSightTo(TargetActor);

	BBComp->SetValueAsBool(WithinRangeKey.SelectedKeyName, bHasLOS && bIsWithinRange);
}
