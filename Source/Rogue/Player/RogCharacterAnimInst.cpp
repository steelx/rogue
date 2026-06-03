// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogCharacterAnimInst.h"

#include "RoguePlayerCharacter.h"
#include "Components/CustomMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URogCharacterAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ARoguePlayerCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		CharMovementComp = OwningCharacter->GetCustomMovementComponent();
	}
}

void URogCharacterAnimInst::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter || !CharMovementComp) return;

	UpdateGroundSpeed();
	UpdateAirSpeed();
	UpdateShouldMove();
	UpdateIsFalling();

	// Strafing if we are not auto-orienting to movement and we use controller yaw
	bIsStrafing = OwningCharacter->bUseControllerRotationYaw && !CharMovementComp->bOrientRotationToMovement;
	Direction = ComputeMovementDirection();
	SetRotations(DeltaSeconds);
}

void URogCharacterAnimInst::SetRotations(float DeltaSeconds)
{
    if (!OwningCharacter || DeltaSeconds <= 0.0f)
    {
        return;
    }

    ActorRotation = OwningCharacter->GetActorRotation();

    const FRotator ControlRot = OwningCharacter->GetControlRotation();
    const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, ActorRotation);

    // Max ensures we don't look too far down
    Pitch = FMath::Max(DeltaRot.Pitch, 0.0f);
    LookYaw = ActorRotation;

    // Velocity & Move Direction Tracking Logic
    const FVector Velocity = OwningCharacter->GetVelocity();
    const FRotator VelocityRot = UKismetMathLibrary::MakeRotFromX(Velocity);
    const FRotator MoveDelta = UKismetMathLibrary::NormalizedDeltaRotator(VelocityRot, ControlRot);
    MoveDirectionYaw = MoveDelta.Yaw;

    PrevLookYaw = ActorRotation;

    // Turn Evaluation Block
    const FRotator DynamicDelta = UKismetMathLibrary::NormalizedDeltaRotator(LookYaw, PrevLookYaw);
    const float TargetDegrees = DynamicDelta.Yaw;

    // ClampAngle node values matching [-1.8, 1.8] configuration
    const float ClampedAngle = FMath::ClampAngle(TargetDegrees, -1.8f, 1.8f);
    LookYawDelta.Yaw = ClampedAngle;

    Yaw = LookYawDelta.Yaw * 25.0f;

    // Branching Evaluation: Velocity State Analysis
    if (IsMoving())
    {
        // Node Group: Take snapshot of last yaw while moving (K2Node_VariableSet_1)
        YawOrigin = ActorRotation.Yaw;

        // Node: blend out the turn pose when we start moving (K2Node_VariableSet_12)
        TurnInPlaceTime = FMath::FInterpTo(TurnInPlaceTime, 0.0f, DeltaSeconds, 12.0f);
    }
    else
    {
        // Node Group: Turn Right Anim Calc Expression Mapping
        const float SubtractionValue = ActorRotation.Yaw - YawOrigin;
        const float NormalizedAngle = UKismetMathLibrary::NormalizeAxis(SubtractionValue);

        // Node: K2Node_MathExpression_0 Evaluation
        // Expression: ((((Delta / TurnSequenceAngle) + 10) * TurnSequenceLength) % TurnSequenceLength)
        const float ExpressionRaw = ((NormalizedAngle / TurnSequenceAngle) + 10.0f) * TurnSequenceLength;
        const float MathExpressionResult = FMath::Fmod(ExpressionRaw, TurnSequenceLength);

        // Node: NotEqual check vs 0.0 before evaluating alpha blend curves
        if (!FMath::IsNearlyEqual(NormalizedAngle, 0.0f, 0.0001f))
        {
            TurnInPlaceTime = MathExpressionResult;

            // Node Group: EdGraphNode_Comment_2 (Calc extremes of turn rotation to blend out)
            // Node: Divide node "normalize 0.0 - 1.0"
            const float NormalizedTurnTime = TurnInPlaceTime / MaxTurnInPlaceTime;
            // Node: Subtract node "-0.5 - +0.5"
            const float CenteredTime = NormalizedTurnTime - 0.5f;
            const float AbsoluteTime = FMath::Abs(CenteredTime);
            // Node: Multiply node "back to 0.0-1.0 (where 0.0 is halfway point)"
            const float HalfwayFactor = AbsoluteTime * 2.0f;

            // Exponential blend adjustments mimicking curve modification nodes
            const float ExpCurve = FMath::Pow(HalfwayFactor, 4.0f);
            TurnInPlaceBlendAlpha = 1.0f - ExpCurve;

            const float TimeDeltaCalculation = FMath::Abs(PrevTurnInPlaceTimeAbs - FMath::Abs(TurnInPlaceTime));
            const float SpeedClampedDelta = FMath::Min(TimeDeltaCalculation, 1.0f);

            const float AdjustedBase = FMath::Abs(SpeedClampedDelta);
            const float PowerModifier = FMath::Pow(AdjustedBase, 0.4f);

            TurnInPlaceTimeDelta = FMath::FInterpTo((TurnInPlaceTimeDelta + PowerModifier), 0.0f, DeltaSeconds, 20.0f);
            PrevTurnInPlaceTimeAbs = FMath::Abs(TurnInPlaceTime);
        }
    }
}

void URogCharacterAnimInst::UpdateGroundSpeed()
{
	GroundSpeed = UKismetMathLibrary::VSizeXY(OwningCharacter->GetVelocity());
}

void URogCharacterAnimInst::UpdateAirSpeed()
{
	AirSpeed = OwningCharacter->GetVelocity().Z;
}

void URogCharacterAnimInst::UpdateShouldMove()
{
	bShouldMove = CharMovementComp->GetCurrentAcceleration().Size() > 0 && GroundSpeed > 5.f && !bIsFalling;
}

bool URogCharacterAnimInst::IsMoving() const
{
	if (OwningCharacter)
	{
		return OwningCharacter->GetVelocity().SizeSquared() > 100.0f;
	}
	return false;
}

void URogCharacterAnimInst::UpdateIsFalling()
{
	bIsFalling = CharMovementComp->IsFalling();
}

/*
* Calculate direction using the delta between the velocity and the actor rotation.
* When the character is not strafing, clamp the value between - and + 45 degrees
* so that backwards animations do not play when turning around, but running into wall looks better.
*/
float URogCharacterAnimInst::ComputeMovementDirection() const
{
	if (!OwningCharacter || !CharMovementComp)
	{
		return 0.f;
	}

	FVector Velocity2D = CharMovementComp->Velocity;
	Velocity2D.Z = 0.f;

	if (Velocity2D.IsNearlyZero(1.f))
	{
		return 0.f;
	}

	// Yaw of movement vs actor facing
	const FRotator MovementRot = Velocity2D.ToOrientationRotator();
	const FRotator ActorRot = OwningCharacter->GetActorRotation();
	const float RawDeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRot, ActorRot).Yaw; // -180..180

	// If not strafing, clamp to avoid backward blends when quickly rotating
	if (!bIsStrafing)
	{
		return FMath::Clamp(RawDeltaYaw, -45.f, 45.f);
	}

	return RawDeltaYaw;
}