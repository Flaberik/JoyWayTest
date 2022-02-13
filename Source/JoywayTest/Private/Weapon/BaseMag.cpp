// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseMag.h"

// Sets default values
ABaseMag::ABaseMag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_MagComponent = CreateDefaultSubobject<UStaticMeshComponent>("SM_MagComponent");
	RootComponent = SM_MagComponent;

	// В родительском классе PickUpMesh используется для включения/выключения физики 
	PickUpMesh = SM_MagComponent;

	BoxDetectMagSocket = CreateDefaultSubobject<UBoxComponent>("BoxDetectMagSocket");
	BoxDetectMagSocket->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseMag::BeginPlay()
{
	Super::BeginPlay();
	UpdateState();
}

// Called every frame
void ABaseMag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseMag::PickUp_Implementation(USceneComponent* AttachTo)
{
	Super::PickUp_Implementation(AttachTo);
	UpdateState();
}

void ABaseMag::UpdateState()
{
	if (BulletAmount <= 0)
	{
		SM_MagComponent->SetStaticMesh(SM_MagEmptyState);
	}
	else
	{
		SM_MagComponent->SetStaticMesh(SM_MagFullState);
	}
}
