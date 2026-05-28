#include "RogStructTypes.h"

#include "Abilities/GameplayAbility.h"

bool FInputAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant != nullptr;
}
