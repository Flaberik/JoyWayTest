// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"

#include "Character/Interfaces/PlayerInterface.h"

// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = RootComp;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(RootComponent);

	PlayerDetector = CreateDefaultSubobject<UBoxComponent>("PlayerDetector");
	PlayerDetector->SetupAttachment(RootComponent);

	PlayerDetector->OnComponentBeginOverlap.AddDynamic(this, &AWeaponSpawner::PlayerDetectorOnOverlapBegin);
	PlayerDetector->OnComponentEndOverlap.AddDynamic(this, &AWeaponSpawner::PlayerDetectorOnOverlapEnd);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootComponent);
}

void AWeaponSpawner::SpawnActor(FActorSpawn& ActorToSpawn)
{
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSpawner::PlayerDetectorOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
	{
		Cast<IPlayerInterface>(OtherActor)->Execute_SetEnabledWidgetInteraction(OtherActor, true);
	}
}

void AWeaponSpawner::PlayerDetectorOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
	{
		Cast<IPlayerInterface>(OtherActor)->Execute_SetEnabledWidgetInteraction(OtherActor, false);
	}
}
