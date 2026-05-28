// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogProjectileMagic.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraFunctionLibrary.h"
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

	if (OtherActor)
	{
		// Apply Damage using Gameplay Effect (GE) to the Target Actor
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor))
		{
			if (UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent())
			{
				if (DamageEffectClass)
				{
					FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
					EffectContext.AddInstigator(GetInstigator(), this);

					// Create the Gameplay Effect Spec
					FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
					if (EffectSpecHandle.IsValid())
					{
						// Pass the damage value dynamically using a SetByCaller tag (e.g., Shared_SetByCaller_BaseDamage)
						EffectSpecHandle.Data->SetSetByCallerMagnitude(MyTags::Shared_SetByCaller_BaseDamage, DamageAmount);
						TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
					}
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
