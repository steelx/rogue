// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogueProjectileBlackhole.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ARogueProjectileBlackhole::ARogueProjectileBlackhole()
{
	PrimaryActorTick.bCanEverTick = false;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(GetRootComponent());
	RadialForceComp->ForceStrength = -800000.f;// negative strength to pull objects inwards
	RadialForceComp->Radius = 1200.f;

	// avoid pawns
	RadialForceComp->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	SphereCollision->SetSphereRadius(20.f);
	SphereCollision->SetCollisionProfileName("OverlapAll");

	MovementComponent->InitialSpeed = 550.f;
}

void ARogueProjectileBlackhole::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ElapsedTimerHandle, this, &ThisClass::ElapsedTimerFunction, ElapsedTimerDelay, false);
}

void ARogueProjectileBlackhole::OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}

void ARogueProjectileBlackhole::ExplosionSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

void ARogueProjectileBlackhole::ElapsedTimerFunction()
{
	LoopedNiagaraComponent->DestroyInstance();
	LoopedNiagaraComponent->Deactivate();
	LoopedAudioComponent->Deactivate();
	MovementComponent->StopMovementImmediately();
	ExplosionNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());

	ExplosionNiagaraComp->OnSystemFinished.AddDynamic(this, &ThisClass::ExplosionSystemFinished);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}
