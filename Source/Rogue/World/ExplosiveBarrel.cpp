// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "ExplosiveBarrel.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("Physics Actor"));
	SetRootComponent(Mesh);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(Mesh);

	// Radial Impulse config
	RadialForceComp->ImpulseStrength = 15000.f;
	RadialForceComp->Radius = 750.f;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
}

float AExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bExploded || GetWorldTimerManager().TimerExists(ExplosionTriggerTimerHandle))
	{
		return ActualDamage;
	}

	Explode();

	return ActualDamage;
}

void AExplosiveBarrel::Explode()
{
	// initiate burning
	ActiveBurningSoundComp = UGameplayStatics::SpawnSoundAttached(ActiveBurningSound, Mesh);
	ActiveBurningEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(ActiveBurningEffect, Mesh, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

	// Explode
	GetWorldTimerManager().SetTimer(ExplosionTriggerTimerHandle, this, &ThisClass::TriggerExplosion, ExplosionTriggerTimerDelay, false);
}

void AExplosiveBarrel::TriggerExplosion()
{
	bExploded = true;
	ActiveBurningEffectComp->Deactivate();
	ActiveBurningSoundComp->Stop();

	RadialForceComp->FireImpulse();
	Mesh->AddImpulse(FVector::UpVector * ExplosionImpuse, NAME_None, true);
	Mesh->AddAngularImpulseInDegrees(FVector::RightVector * ExplosionImpuse, NAME_None, true);

	// Visual and Sound effects
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}
