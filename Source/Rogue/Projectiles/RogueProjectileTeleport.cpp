// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogueProjectileTeleport.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARogueProjectileTeleport::ARogueProjectileTeleport()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARogueProjectileTeleport::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MovementComponent->InitialSpeed = 4500.f;
}

void ARogueProjectileTeleport::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TravelTimerHandle, this, &ThisClass::TravelTimerElapsed, TravelTimerDelay);
}

void ARogueProjectileTeleport::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	GetWorldTimerManager().ClearTimer(TravelTimerHandle);
	TravelTimerElapsed();
}

void ARogueProjectileTeleport::TravelTimerElapsed()
{
	LoopedNiagaraComponent->Deactivate();
	LoopedAudioComponent->Deactivate();
	MovementComponent->StopMovementImmediately();
	ExplosionNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());

	ExplosionNiagaraComp->OnSystemFinished.AddDynamic(this, &ThisClass::ExplosionSystemFinished);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}

// Handle Pawn teleportation
void ARogueProjectileTeleport::ExplosionSystemFinished(UNiagaraComponent* PSystem)
{
	APawn* PawnToTeleport = GetInstigator();
	if (!IsValid(PawnToTeleport)) return;

	PawnToTeleport->TeleportTo(GetActorLocation(), GetActorRotation());
	Destroy();
}
