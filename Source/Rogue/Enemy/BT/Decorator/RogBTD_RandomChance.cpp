// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogBTD_RandomChance.h"

URogBTD_RandomChance::URogBTD_RandomChance()
{
	NodeName = "Random Chance";
	SuccessProbability = 0.5f; // Default to 50% chance
}

bool URogBTD_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// FMath::FRand() generates a random float between 0.0 and 1.0
	return FMath::FRand() <= SuccessProbability;
}
