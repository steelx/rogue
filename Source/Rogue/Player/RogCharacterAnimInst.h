// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RogCharacterAnimInst.generated.h"

class UCustomMovementComponent;
class ARoguePlayerCharacter;
/**
 *
 */
UCLASS()
class ROGUE_API URogCharacterAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// State Persistence Properties matching Graph Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	FRotator ActorRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	FRotator PrevLookYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	FRotator LookYawDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	FRotator LookYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float MoveDirectionYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float TurnInPlaceTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float TurnInPlaceTimeDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float TurnInPlaceBlendAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float PrevTurnInPlaceTimeAbs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation State")
	float YawOrigin;

	// Configurable parameters derived from node constant pins
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation Config")
	float TurnSequenceAngle = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation Config")
	float TurnSequenceLength = 1.0f; // Bound by node calculations

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData|Rotation Config")
	float MaxTurnInPlaceTime = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Animation|Rotations")
	void SetRotations(float DeltaSeconds);
private:
	UPROPERTY()
	TObjectPtr<ARoguePlayerCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UCustomMovementComponent> CharMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess=true))
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess=true))
	float AirSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess=true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess=true))
	bool bIsStrafing{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess=true))
	bool bShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess=true))
	bool bIsFalling;

	void UpdateGroundSpeed();
	void UpdateAirSpeed();
	void UpdateShouldMove();
	void UpdateIsFalling();
	float ComputeMovementDirection() const;
	bool IsMoving() const;
};
