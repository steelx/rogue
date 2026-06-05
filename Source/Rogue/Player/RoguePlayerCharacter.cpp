// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "RogPlayerState.h"
#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CustomMovementComponent.h"
#include "Core/RogueGameTypes.h"
#include "DataAssets/StartupData/DataAsset_StartupDataBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogProjectileMagic.h"
#include "Projectiles/RogueProjectileBlackhole.h"
#include "Projectiles/RogueProjectileTeleport.h"

TAutoConsoleVariable<bool> CVarLineTraceDebugDrawing(TEXT("game.lineTrace.DebugDraw"), false, TEXT("Enable line trace drawing. (0 = off, 1 = enabled)"), ECVF_Cheat);

ARoguePlayerCharacter::ARoguePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;// true; Rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacterMovement());
}

UAbilitySystemComponent* ARoguePlayerCharacter::GetAbilitySystemComponent() const
{
	return GetRogAbilitySystemComponent();
}

URogAbilitySystemComponent* ARoguePlayerCharacter::GetRogAbilitySystemComponent() const
{
	const ARogPlayerState* RogPlayerState = Cast<ARogPlayerState>(GetPlayerState());
	if (!IsValid(RogPlayerState)) return nullptr;

	return RogPlayerState->GetRogAbilitySystemComponent();
}

URogAttributeSet* ARoguePlayerCharacter::GetAttributeSet() const
{
	const ARogPlayerState* RogPlayerState = Cast<ARogPlayerState>(GetPlayerState());
	if (!IsValid(RogPlayerState)) return nullptr;

	return RogPlayerState->GetAttributeSet();
}

void ARoguePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsValid(GetAbilitySystemComponent())) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);

	ensureMsgf(!StartupData.IsNull(), TEXT("StartupData is null in %s. Please assign a valid DataAsset_StartupDataBase."), *GetName());

	if (!StartupData.IsNull())
	{
		if (UDataAsset_StartupDataBase* LoadedData = StartupData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(GetRogAbilitySystemComponent());
			// UE_LOG(LogTemp, Warning, TEXT("AHeroCharacter:PossessedBy Loaded Startup Data: %s"), *GetName());
		}
	}
}

void ARoguePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (!IsValid(GetAbilitySystemComponent())) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
}

// Called to bind functionality to input
void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::JumpInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);

		// Climbing
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ThisClass::OnClimbActionStarted);
		EnhancedInputComponent->BindAction(ClimbHopAction, ETriggerEvent::Started, this, &ThisClass::OnClimbHopActionStarted);

		// TODO: Move these to IMC_Abilities & GAS Ability
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::HandlePrimaryAttack);
		EnhancedInputComponent->BindAction(SuperAttackAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::HandleSuperAttack);
		EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::HandleTeleportAction);
	}
}

void ARoguePlayerCharacter::JumpInput(const FInputActionValue& Value) {Jump();}

void ARoguePlayerCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	const FVector2D InputValue = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(InputValue.Y);
	}
}

void ARoguePlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	if (!CustomMovementComponent) return;
	if (CustomMovementComponent->IsClimbing())
	{
		HandleClimbMovementInput(Value);
	}
	else
	{
		HandleGroundMovementInput(Value);
	}

	/**
	* IA_Move mappings:
		W (Forward)
			Modifiers (1): Swizzle Input Axis Values

		S (Backward)
			Modifiers (2): Swizzle Input Axis Values, then Negate

		A (Left)
			Modifiers (1): Negate

		D (Right)
			Modifiers (0): (Delete any existing modifiers so the array is empty)
	 */
}

void ARoguePlayerCharacter::HandleGroundMovementInput(const FInputActionValue & Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	//this logic expects Y to drive Forward/Backward and X to drive Left/Right.
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARoguePlayerCharacter::HandleClimbMovementInput(const FInputActionValue & Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// This is Up Vector, hence pressing W will move Upwards
	const FVector ForwardDirection = FVector::CrossProduct(
		-CustomMovementComponent->GetClimbableSurfaceNormal(),
		GetActorRightVector()
	);

	const FVector RightDirection = FVector::CrossProduct(
		-CustomMovementComponent->GetClimbableSurfaceNormal(),
		-GetActorUpVector()
	);

	// add movement
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ARoguePlayerCharacter::OnClimbActionStarted(const FInputActionValue & Value)
{
	if(!CustomMovementComponent) return;
	CustomMovementComponent->ToggleClimbing(!CustomMovementComponent->IsClimbing());
}


void ARoguePlayerCharacter::OnClimbHopActionStarted(const FInputActionValue & Value)
{
	if(CustomMovementComponent)
	{
		CustomMovementComponent->RequestHopping();
	}
}


void ARoguePlayerCharacter::HandlePrimaryAttack()
{
	if (!IsValid(ProjectileMagicClass)) return;

	PlayAnimMontage(PrimaryAttackAnim);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		PrimaryAttackCastingEffect,
		GetMesh(), PrimaryAttackSocket,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
		true
	);

	UGameplayStatics::PlaySound2D(this, PrimaryAttackCastingSound);

	FTimerHandle AttackTimerHandle;
	constexpr float AttackTimerDelay = 0.2f;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::PrimaryAttackTimerElapsed, AttackTimerDelay, false);
}

void ARoguePlayerCharacter::HandleSuperAttack()
{
	if (!IsValid(ProjectileBlackholeClass)) return;

	PlayAnimMontage(PrimaryAttackAnim);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		SuperAttackCastingEffect,
		GetMesh(), PrimaryAttackSocket,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
		true
	);

	UGameplayStatics::PlaySound2D(this, SuperAttackCastingSound);

	FTimerHandle AttackTimerHandle;
	constexpr float AttackTimerDelay = 0.2f;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::SuperAttackTimerElapsed, AttackTimerDelay, false);
}

void ARoguePlayerCharacter::HandleTeleportAction()
{
	if (!IsValid(ProjectileTeleportClass)) return;

	PlayAnimMontage(PrimaryAttackAnim);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		PrimaryAttackCastingEffect,
		GetMesh(), PrimaryAttackSocket,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
		true
	);

	UGameplayStatics::PlaySound2D(this, PrimaryAttackCastingSound);

	FTimerHandle AttackTimerHandle;
	constexpr float AttackTimerDelay = 0.2f;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::TeleportActionTimerElapsed, AttackTimerDelay, false);
}

void ARoguePlayerCharacter::TeleportActionTimerElapsed()
{
	const FVector Location = GetMesh()->GetSocketLocation(PrimaryAttackSocket);
	const FRotator Rotation = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARogueProjectileTeleport* ProjectileTeleport = GetWorld()->SpawnActor<ARogueProjectileTeleport>(ProjectileTeleportClass, Location, Rotation, SpawnParams);
	MoveIgnoreActorAdd(ProjectileTeleport);
}

void ARoguePlayerCharacter::PrimaryAttackTimerElapsed()
{
	UWorld* World = GetWorld();
	const FVector SpawnLocation = GetMesh()->GetSocketLocation(PrimaryAttackSocket);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	constexpr float LineTraceDist = 3000.f;
	const FVector EyeLocation = CameraComponent->GetComponentLocation();
	const FRotator EyeRotation = GetControlRotation();
	const FVector TraceEndLocation = EyeLocation + (EyeRotation.Vector()*LineTraceDist);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);


	FHitResult Hit;
	const bool bBlockingHit = World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEndLocation, PROJECTILE_OBJ_CHANNEL, QueryParams);
	FVector AdjustedLocation = bBlockingHit ? Hit.Location : TraceEndLocation;

	FRotator SpawnRotation = (AdjustedLocation - SpawnLocation).Rotation();
	ARogProjectileMagic* ProjectileMagic = World->SpawnActor<ARogProjectileMagic>(ProjectileMagicClass, SpawnLocation, SpawnRotation, SpawnParams);
	MoveIgnoreActorAdd(ProjectileMagic);

	const bool bDebugDraw = CVarLineTraceDebugDrawing.GetValueOnGameThread();
	if (bDebugDraw)
	{
		DrawDebugBox(World, AdjustedLocation, FVector(20.f), FColor::Emerald,false, 5.f);// Line trace HIT location
		DrawDebugLine(World, EyeLocation, TraceEndLocation, FColor::Green, false, 5.f);// without hit path
		DrawDebugLine(World, SpawnLocation, AdjustedLocation, FColor::Yellow, false, 5.f);// adjusted projectile path
		DrawDebugLine(World, SpawnLocation, SpawnLocation+(GetControlRotation().Vector()*LineTraceDist), FColor::Red, false, 5.f);// Original projectile path
	}
}

void ARoguePlayerCharacter::SuperAttackTimerElapsed()
{
	const FVector Location = GetMesh()->GetSocketLocation(PrimaryAttackSocket);
	const FRotator Rotation = GetActorForwardVector().Rotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARogueProjectileBlackhole* ProjectileBlackhole = GetWorld()->SpawnActor<ARogueProjectileBlackhole>(ProjectileBlackholeClass, Location, Rotation, SpawnParams);
	MoveIgnoreActorAdd(ProjectileBlackhole);
}

// void ARoguePlayerCharacter::HandleHealthChanged(float NewHealth, float OldHealth)
// {
// 	// Died?
// 	if (FMath::IsNearlyZero(NewHealth))
// 	{
// 		DisableInput(nullptr);
// 		GetMovementComponent()->StopMovementImmediately();
// 		PlayAnimMontage(DeathAnimMontage);
// 	}
// }
