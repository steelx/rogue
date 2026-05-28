// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBaseCharacter.h"



// Sets default values
ARogBaseCharacter::ARogBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}


UAbilitySystemComponent* ARogBaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}
