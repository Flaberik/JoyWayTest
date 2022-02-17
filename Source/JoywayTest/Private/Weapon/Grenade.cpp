// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade.h"

#include "Kismet/GameplayStatics.h"

AGrenade::AGrenade()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	SM_Grenade = CreateDefaultSubobject<UStaticMeshComponent>("SM_Grenade");
	SM_Grenade->SetupAttachment(RootComponent);

	PickUpMesh = SM_Grenade;
}

void AGrenade::BlowUp()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, SM_Grenade->GetComponentTransform());
	TArray<AActor*> ActorsIgnore;

	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, SM_Grenade->GetComponentLocation(), RadialDamageRadius,
	                                    UDamageType::StaticClass(), ActorsIgnore);
	
	Destroy();
}

void AGrenade::Drop_Implementation(AActor* Hand)
{
	Super::Drop_Implementation(Hand);
	GetWorld()->GetTimerManager().SetTimer(BlowUpTimer, this, &AGrenade::BlowUp, 0.1f, false,
	                                       DelayBeforeExplosion);
}

void AGrenade::PickUp_Implementation(USceneComponent* AttachTo)
{
	Super::PickUp_Implementation(AttachTo);
	
	if (AttachTo != nullptr)
	{
		SM_Grenade->SetWorldLocation(AttachTo->GetComponentLocation());
	}
		
}
