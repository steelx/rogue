#include "MyTags.h"


namespace MyTags
{
	/** InputTags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_None, "MyTags.InputTag.None");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact, "MyTags.InputTag.Interact");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Heavy, "MyTags.InputTag.Attack.Heavy");


	/** Events **/
	UE_DEFINE_GAMEPLAY_TAG(Event_TargetPawn_Spotted, "MyTags.Event.TargetPawn.Spotted");
	UE_DEFINE_GAMEPLAY_TAG(Event_TargetPawn_Lost, "MyTags.Event.TargetPawn.Lost");
	UE_DEFINE_GAMEPLAY_TAG(Event_Owner_RotationFinished, "MyTags.Event.Owner.RotationFinished");
	UE_DEFINE_GAMEPLAY_TAG(Event_Owner_Attack_Ranged, "MyTags.Event.Owner.Attack.Ranged");

	/** Shared Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "MyTags.Shared.SetByCaller.BaseDamage");
}
