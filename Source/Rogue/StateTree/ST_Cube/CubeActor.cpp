// Copyright 2026 Ajinkya Borade. All Rights Reserved.


#include "CubeActor.h"

#include "Components/SphereComponent.h"
#include "Components/StateTreeComponent.h"
#include "Components/TimelineComponent.h"
#include "Core/MyTags.h"


ACubeActor::ACubeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	SetRootComponent(CubeMesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(CubeMesh);
	SphereCollision->SetSphereRadius(300.f);

	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));

	LookAtRotationTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("LookAtRotationTimelineComp"));
}

void ACubeActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	FOnTimelineFloat LookAtTimelineUpdate;
	LookAtTimelineUpdate.BindUFunction(this, "LookAtTimelineUpdate");
	LookAtRotationTimelineComp->AddInterpFloat(LookFloatCurve, LookAtTimelineUpdate);

	FOnTimelineEvent LookAtTimelineFinished;
	LookAtTimelineFinished.BindUFunction(this, "LookAtTimelineFinished");
	LookAtRotationTimelineComp->SetTimelineFinishedFunc(LookAtTimelineFinished);
}

void ACubeActor::LookAt(const FRotator& InLookAtRotation)
{
	LookAtRotation = InLookAtRotation;
	LookAtRotationTimelineComp->PlayFromStart();
}

void ACubeActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* SpottedPawn = Cast<APawn>(OtherActor))
	{
		TargetPawn = SpottedPawn;

		// Send event to State Tree to switch to "Chasing" state
		FStateTreeEvent PredEvent;
		PredEvent.Tag = MyTags::Event_TargetPawn_Spotted;
		StateTreeComponent->SendStateTreeEvent(PredEvent);
		UE_LOG(LogTemp, Warning, TEXT("Sensed: Event_TargetPawn_Spotted"));
	}
}

void ACubeActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (Cast<APawn>(OtherActor) == TargetPawn)
	{
		TargetPawn = nullptr;

		// Send event to State Tree to exit to "Chasing" state
		FStateTreeEvent StateTreeEvent;
		StateTreeEvent.Tag = MyTags::Event_TargetPawn_Lost;
		StateTreeComponent->SendStateTreeEvent(StateTreeEvent);
		UE_LOG(LogTemp, Warning, TEXT("Sent: Event_TargetPawn_Lost"));
	}
}

void ACubeActor::LookAtTimelineUpdate(float Alpha)
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), LookAtRotation, Alpha));
}

void ACubeActor::LookAtTimelineFinished()
{
	LookAtRotation = FRotator::ZeroRotator;
	SetActorRotation(LookAtRotation);
	LookChance -= FMath::RandRange(0.1f, 0.3f);

	FStateTreeEvent StateTreeEvent;
	StateTreeEvent.Tag = MyTags::Event_Owner_RotationFinished;
	StateTreeComponent->SendStateTreeEvent(StateTreeEvent);
	// UE_LOG(LogTemp, Warning, TEXT("Sent: Event_Owner_RotationFinished"));
}
