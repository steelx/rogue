// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogProjectileMagic.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/RogAbilitySystemComponent.h"
#include "Core/MyTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogProjectileMagic::ARogProjectileMagic()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARogProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ARogProjectileMagic::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	// const FVector HitFromDirection = GetActorRotation().Vector();
	// UGameplayStatics::ApplyPointDamage(OtherActor, 10.f, HitFromDirection, Hit, GetInstigatorController(), this, DamageTypeClass);

	// Target who got HIT should not create its own EffectSpec, but the instigator (the one dealing the damage).
	if (OtherActor && DamageEffectClass)
	{
		IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(OtherActor);
		IAbilitySystemInterface* InstigatorASI = Cast<IAbilitySystemInterface>(GetInstigator());
		if (TargetASI && InstigatorASI)
		{
			URogAbilitySystemComponent* TargetASC = Cast<URogAbilitySystemComponent>(TargetASI->GetAbilitySystemComponent());
			URogAbilitySystemComponent* InstigatorASC = Cast<URogAbilitySystemComponent>(InstigatorASI->GetAbilitySystemComponent());
			if (TargetASC && InstigatorASC)
			{
				const FGameplayEffectSpecHandle EffectSpec = InstigatorASC->MakeDamageEffectSpec(DamageEffectClass, DamageAmount);
				if (EffectSpec.IsValid())
				{
					InstigatorASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);
				}
			}
		}
	}

	// Visuals & Destruction
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	// destroy projectile
	Destroy();
}
