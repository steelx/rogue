// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogRangedAttackNotify.h"

#include "Enemy/RogEnemyCharacter.h"

void URogRangedAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ARogEnemyCharacter* Character = Cast<ARogEnemyCharacter>(MeshComp->GetOwner()))
	{
		Character->OnRangedAttackEvent.Broadcast();
	}
}
