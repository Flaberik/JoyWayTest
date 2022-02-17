// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletProjectTile.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Weapon/DamageIndicator.h"

ABulletProjectTile::ABulletProjectTile()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));

	GetStaticMeshComponent()->bReturnMaterialOnMove = true;
	GetStaticMeshComponent()->OnComponentHit.AddDynamic(this, &ABulletProjectTile::OnHit);

	// this->OnActorHit.AddDynamic(this, &ABulletProjectTile::OnHit);
	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &ABulletProjectTile::ProjectileStop);
}

void ABulletProjectTile::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletProjectTile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(ProjectileMovementComponent->Velocity);
	}

	if (OtherActor != nullptr)
	{
		// Если есть физ материал наносим урон и спавним виджет (Индиктор урона)
		if (Hit.PhysMaterial.Get())
		{
			MultiplayDamageByPhysMat(Hit.PhysMaterial.Get());
			UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this,
			                              UDamageType::StaticClass());
			FRotator WidgetRotator = UKismetMathLibrary::FindLookAtRotation(
				Hit.ImpactPoint, GetOwner()->GetActorLocation());
			FVector WidgetLocation = Hit.ImpactPoint + (GetOwner()->GetActorLocation() - Hit.ImpactPoint).Normalize() *
				10.0f;
			
			SpawnWidgetDamageIndicator(WidgetLocation, WidgetRotator);
		}

		SpawnHitEffectEmmiter(Hit);
		
	}
}

void ABulletProjectTile::SpawnHitEffectEmmiter(const FHitResult& Hit)
{
	if(ParticleSystemHitEffect == nullptr)
		return;
	
	FTransform SpawnTransform = FTransform( Hit.ImpactNormal.Rotation(),Hit.ImpactPoint);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ParticleSystemHitEffect, SpawnTransform);
}

void ABulletProjectTile::MultiplayDamageByPhysMat(UPhysicalMaterial* PhysMat)
{
	if (PhysMat)
	{
		Damage = PhysMat->DestructibleDamageThresholdScale * Damage;
	}
	return;
}

void ABulletProjectTile::DestroyProjectile()
{
	this->Destroy();
}

void ABulletProjectTile::ProjectileStop(const FHitResult& ImpactResult)
{
	DestroyProjectile();
}

void ABulletProjectTile::SpawnWidgetDamageIndicator(FVector WorldLocation, FRotator WorldRotation)
{
	FTransform SpawnTransform = FTransform(WorldRotation, WorldLocation);

	ADamageIndicator* DamageIndicatorInstance = GetWorld()->SpawnActorDeferred<ADamageIndicator>(
		DamageIndicator, SpawnTransform, GetOwner());

	DamageIndicatorInstance->Damage = Damage;
	DamageIndicatorInstance->FinishSpawning(SpawnTransform);
}
