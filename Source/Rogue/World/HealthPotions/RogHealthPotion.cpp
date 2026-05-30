// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogHealthPotion.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Core/MyTags.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RoguePlayerCharacter.h"


ARogHealthPotion::ARogHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereCollision);

	LoopingAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopingAudioComp"));
	LoopingAudioComp->SetupAttachment(SphereCollision);
	MeshAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MeshAnimTimeline"));
}

void ARogHealthPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FOnTimelineFloat MeshAnimTimelineUpdateFunc;
	MeshAnimTimelineUpdateFunc.BindUFunction(this, FName("MeshAnimTimelineUpdate"));
	MeshAnimTimeline->AddInterpFloat(MeshAnimCurveFloat, MeshAnimTimelineUpdateFunc);

	FOnTimelineEvent MeshAnimTimelineFinishedFunc;
	MeshAnimTimelineFinishedFunc.BindUFunction(this, FName("MeshAnimTimelineFinished"));
	MeshAnimTimeline->SetTimelineFinishedFunc(MeshAnimTimelineFinishedFunc);
}

void ARogHealthPotion::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnActorOverlapped(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// we will only allow Player Character to heal
	ARoguePlayerCharacter* Player = Cast<ARoguePlayerCharacter>(OtherActor);
	if (!IsValid(Player)) return;

	// if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor))
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
	if (!ASC || !IsValid(HealingEffectClass)) return;

	// Apply Damage Gameplay Effect via ASC
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(HealingEffectClass, 1, ContextHandle);

	if (EffectSpecHandle.IsValid())
	{
		// Dynamically set the healing magnitude
		EffectSpecHandle.Data->SetSetByCallerMagnitude(MyTags::Shared_SetByCaller_BaseDamage, HealAmount);
		ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}

	// VFX
	LoopingAudioComp->Deactivate();

	UGameplayStatics::PlaySoundAtLocation(this, HealingSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		HealingEffect, Player->GetMesh(), FName("Muzzle_01"),
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
	true
	);

	Destroy();
}

void ARogHealthPotion::MeshAnimTimelineFinished()
{
	MeshComp->SetRelativeLocation(FVector::ZeroVector);
}

void ARogHealthPotion::MeshAnimTimelineUpdate(float Alpha)
{
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z += Alpha;
	MeshComp->SetRelativeLocation(CurrentLocation);
}
