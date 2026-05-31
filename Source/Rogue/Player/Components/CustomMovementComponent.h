// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

DECLARE_DELEGATE(FOnEnterClimbState)
DECLARE_DELEGATE(FOnExitClimbState)

UENUM(BlueprintType)
namespace ECustomMovementMode
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName = "Climb Mode")
	};
}

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API UCustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FOnEnterClimbState OnEnterClimbStateDelegate;
	FOnExitClimbState OnExitClimbStateDelegate;

	UFUNCTION(BlueprintCallable)
	void ToggleClimbing(const bool bEnableClimb);
	void RequestHopping();
	FVector GetUnrotatedClimbVelocity() const;
	bool IsClimbing() const;
	FORCEINLINE FVector GetClimbableSurfaceNormal() const {return CurrentClimbableSurfaceNormal;}

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

#pragma region Climbing

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery> > ClimbSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceHalfHeight = 72.f;

#pragma endregion

	/**
	 * Traces forward from the character to detect climbable surfaces.
	 * Stores the results in ClimbableSurfacesTracedResults.
	 * @return True if at least one climbable surface was detected.
	 */
	bool TraceClimbableSurfaces();
	bool TraceFromEyeHeight(const float TraceDistance, const float StartOffset = 0);
	bool CanStartClimbing();
	void StartClimbing();
	void StopClimbing();

private:
	TArray<FHitResult> ClimbableSurfacesTracedResults;
	FVector CurrentClimbableSurfaceLocation;
	FVector CurrentClimbableSurfaceNormal;

	UPROPERTY()
	TObjectPtr<UAnimInstance> OwningPlayerAnimInstance;

	UPROPERTY()
	TObjectPtr<ACharacter> OwningPlayerCharacter;

	bool bIsClimbing {false};

	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector & Start, const FVector & End, const bool bShowDebugShape = false, const bool bDrawPersistentShape = false) const;
	FHitResult DoLineTraceSingleByObject(const FVector & Start, const FVector & End, const bool bShowDebugShape = false, const bool bDrawPersistentShape = false) const;

};
