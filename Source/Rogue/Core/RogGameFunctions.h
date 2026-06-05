// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RogGameFunctions.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_API URogGameFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static TArray<FHitResult> DoCapsuleTraceMultiByObject(UObject* WorldContextObject, const FVector& Start, const FVector& End, TArray<TEnumAsByte<EObjectTypeQuery>> TraceTypes, const TArray<AActor*>& ActorsToIgnore, const float TraceRadius = 50, const float HalfHeight = 72, const bool bShowDebugShape = false, const bool bDrawPersistentShape = false);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static FHitResult DoLineTraceSingleByObject(UObject* WorldContextObject, const FVector& Start, const FVector& End, TArray<TEnumAsByte<EObjectTypeQuery>> TraceTypes, const TArray<AActor*>& ActorsToIgnore, const bool bShowDebugShape = false, const bool bDrawPersistentShape = false);
};
