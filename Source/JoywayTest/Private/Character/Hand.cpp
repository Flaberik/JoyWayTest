// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hand.h"

// Sets default values
AHand::AHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	MotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	MotionControllerComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

