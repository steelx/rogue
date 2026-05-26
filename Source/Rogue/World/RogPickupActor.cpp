// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "RogPickupActor.h"

#include "Components/SphereComponent.h"


ARogPickupActor::ARogPickupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(128.f);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ARogPickupActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ARogPickupActor::OnActorOverlapped);
}

void ARogPickupActor::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ARogPickupActor::OnActorOverlapped %s"), *OtherActor->GetName());
}
