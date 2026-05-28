// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogPlayerState.h"
#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/RogueGameTypes.h"
#include "DataAssets/StartupData/DataAsset_StartupDataBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogProjectileMagic.h"
#include "Projectiles/RogueProjectileBlackhole.h"
#include "Projectiles/RogueProjectileTeleport.h"

TAutoConsoleVariable<bool> CVarLineTraceDebugDrawing(TEXT("game.lineTrace.DebugDraw"), false, TEXT("Enable line trace drawing. (0 = off, 1 = enabled)"), ECVF_Cheat);

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}

UAbilitySystemComponent* ARoguePlayerCharacter::GetAbilitySystemComponent() const
{
	const ARogPlayerState* RogPlayerState = Cast<ARogPlayerState>(GetPlayerState());
	if (!IsValid(RogPlayerState)) return nullptr;

	return RogPlayerState->GetAbilitySystemComponent();
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
			UE_LOG(LogTemp, Warning, TEXT("AHeroCharacter:PossessedBy Loaded Startup Data: %s"), *GetName());
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

		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::HandlePrimaryAttack);
		EnhancedInputComponent->BindAction(SuperAttackAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::HandleSuperAttack);
		EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::HandleTeleportAction);
	}
}


void ARoguePlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	const FVector2D InputValue = Value.Get<FVector2D>();

	// pass the axis values to the move input
	AddMovementInput(GetActorForwardVector(), InputValue.X);
	AddMovementInput(GetActorRightVector(), InputValue.Y);
}

void ARoguePlayerCharacter::JumpInput(const FInputActionValue& Value)
{
	Jump();
}

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
