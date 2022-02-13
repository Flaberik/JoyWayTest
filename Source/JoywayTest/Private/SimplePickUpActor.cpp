// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplePickUpActor.h"

#include "HandInterface.h"

// Sets default values
ASimplePickUpActor::ASimplePickUpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimplePickUpActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimplePickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimplePickUpActor::PickUp_Implementation(USceneComponent* AttachTo)
{
	IPickUpInterface::PickUp_Implementation(AttachTo);
	PickUpMesh->SetSimulatePhysics(false);
	PickUpMesh->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepWorldTransform);
	SetActorLocationAndRotation(AttachTo->GetComponentLocation(), AttachTo->GetComponentRotation());

	SetOwner(AttachTo->GetOwner());
	
}

void ASimplePickUpActor::Drop_Implementation(AActor* Hand)
{
	IPickUpInterface::Drop_Implementation(Hand);
	
	if(Hand == nullptr)
		return;
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PickUpMesh->SetSimulatePhysics(true);

	if (Hand->GetClass()->ImplementsInterface(UHandInterface::StaticClass()))
	{
		Cast<IHandInterface>(Hand)->Execute_ReleaseActor(Hand);
	}
	
}
