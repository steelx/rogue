// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

class UAnimInstance;
class UAnimMontage;

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
	UCustomMovementComponent();
	FOnEnterClimbState OnEnterClimbStateDelegate;
	FOnExitClimbState OnExitClimbStateDelegate;

	UFUNCTION(BlueprintCallable)
	void ToggleClimbing(const bool bEnableClimb);
	void RequestHopping();
	FVector GetUnrotatedClimbVelocity() const;
	bool IsClimbing() const;

	FORCEINLINE FVector GetClimbableSurfaceNormal() const {return CurrentClimbableSurfaceNormal;}

protected:
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;

#pragma region Climbing

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	TArray<TEnumAsByte<EObjectTypeQuery> > ClimbSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float MaxClimbSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float ClimbCapsuleHalfHeight = 48.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float DefaultCapsuleHalfHeight = 92.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float ClimbCapsuleTraceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float ClimbCapsuleTraceHalfHeight = 72.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float MaxBreakClimbDeceleration = 400.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement|Climbing")
	float MaxClimbAcceleration = 300.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Character Movement")
	TObjectPtr<UAnimMontage> IdleToClimbMontage;

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
	void PhysicsClimb(const float DeltaTime, const int32 Iterations);
	void ProcessClimbableSurfaceInfo();
	bool CheckShouldStopClimbing() const;
	FQuat GetClimbRotation(const float DeltaTime) const;
	void SnapMovementToClimbableSurfaces(const float DeltaTime) const;
	void PlayClimbMontage(UAnimMontage* MontageToPlay) const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	TArray<FHitResult> ClimbableSurfacesTracedResults;// result from DoCapsuleTraceMultiByObject(...)
	FVector CurrentClimbableSurfaceLocation;
	FVector CurrentClimbableSurfaceNormal;

	UPROPERTY()
	TObjectPtr<UAnimInstance> OwningPlayerAnimInstance;

	bool bIsClimbing {false};

	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector & Start, const FVector & End, const bool bShowDebugShape = false, const bool bDrawPersistentShape = false) const;
	FHitResult DoLineTraceSingleByObject(const FVector & Start, const FVector & End, const bool bShowDebugShape = false, const bool bDrawPersistentShape = false) const;

};
