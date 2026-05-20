// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogProjectileMagic.h"
#include "Projectiles/RogueProjectileBlackhole.h"


// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ARoguePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARoguePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ARoguePlayerCharacter::PrimaryAttackTimerElapsed()
{
	const FVector Location = GetMesh()->GetSocketLocation(PrimaryAttackSocket);
	const FRotator Rotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARogProjectileMagic* ProjectileMagic = GetWorld()->SpawnActor<ARogProjectileMagic>(ProjectileMagicClass, Location, Rotation, SpawnParams);
	MoveIgnoreActorAdd(ProjectileMagic);
}

void ARoguePlayerCharacter::SuperAttackTimerElapsed()
{
	const FVector Location = GetMesh()->GetSocketLocation(PrimaryAttackSocket);
	const FRotator Rotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARogueProjectileBlackhole* ProjectileBlackhole = GetWorld()->SpawnActor<ARogueProjectileBlackhole>(ProjectileBlackholeClass, Location, Rotation, SpawnParams);
	MoveIgnoreActorAdd(ProjectileBlackhole);
}

