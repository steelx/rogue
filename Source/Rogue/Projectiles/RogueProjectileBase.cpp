// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogueProjectileBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileBase::ARogueProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;


	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(16.f);
	SphereCollision->SetCollisionProfileName(TEXT("Projectile"));

	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(GetRootComponent());

	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComponent"));

	// Movement & Speed
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 1600.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ARogueProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ARogueProjectileBase::OnComponentOverlapBegin);
	SphereCollision->OnComponentHit.AddDynamic(this, &ARogueProjectileBase::OnComponentHit);
	SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);
}



void ARogueProjectileBase::OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor: %s"), *OtherActor->GetName());
	}
}

void ARogueProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}
