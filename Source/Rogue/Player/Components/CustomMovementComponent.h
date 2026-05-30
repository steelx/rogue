// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API UCustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCustomMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
#pragma region Climbing

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery> > ClimbSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceHalfHeight = 72.f;

#pragma endregion

	void TraceClimbableSurfaces();
	void TraceFromEyeHeight(const float TraceDistance, const float StartOffset = 0);

private:
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector & Start, const FVector & End, const bool bShowDebugShape = false) const;
	FHitResult DoLineTraceSingleByObject(const FVector & Start, const FVector & End, const bool bShowDebugShape = false) const;

};
