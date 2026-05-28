#pragma once

#include "NativeGameplayTags.h"

namespace MyTags
{
	/** InputTags **/
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_None);
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Heavy);


	/** Events **/
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_TargetPawn_Spotted);
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_TargetPawn_Lost);
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Owner_RotationFinished);
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Owner_Attack_Ranged);

	/** Shared Tags **/
	ROGUE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);
};
