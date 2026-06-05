// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogGameFunctions.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FHitResult> URogGameFunctions::DoCapsuleTraceMultiByObject(UObject* WorldContextObject, const FVector& Start, const FVector& End, TArray<TEnumAsByte<EObjectTypeQuery>> TraceTypes, const TArray<AActor*>& ActorsToIgnore, const float TraceRadius, const float HalfHeight, const bool bShowDebugShape, const bool bDrawPersistentShape)
{
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	if (bShowDebugShape)
	{
		DrawDebugType = bDrawPersistentShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::ForOneFrame;
	}
	TArray<FHitResult> OutCapsuleTrace;

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		WorldContextObject, Start, End, TraceRadius, HalfHeight, TraceTypes,
		false, ActorsToIgnore, DrawDebugType, OutCapsuleTrace, true);

	return OutCapsuleTrace;
}

FHitResult URogGameFunctions::DoLineTraceSingleByObject(UObject* WorldContextObject, const FVector& Start, const FVector& End, TArray<TEnumAsByte<EObjectTypeQuery>> TraceTypes, const TArray<AActor*>& ActorsToIgnore, const bool bShowDebugShape, const bool bDrawPersistentShape)
{
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	if (bShowDebugShape)
	{
		DrawDebugType = bDrawPersistentShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::ForOneFrame;
	}
	FHitResult OutLineTrace;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		WorldContextObject, Start, End, TraceTypes, false, ActorsToIgnore,
		DrawDebugType, OutLineTrace, true);

	return OutLineTrace;
}
