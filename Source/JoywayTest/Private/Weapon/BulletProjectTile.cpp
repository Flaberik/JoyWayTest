// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletProjectTile.h"

#include "Kismet/GameplayStatics.h"
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
	if (HitComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
		                                 FString::Printf(TEXT("%s, %i"), *HitComp->GetName(), int(HitComp->IsSimulatingPhysics())));
	}
	if (HitComp->IsSimulatingPhysics())
	{
		HitComp->AddImpulse(ProjectileMovementComponent->Velocity);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
		                                 FString::Printf(
			                                 TEXT("%s"), *ProjectileMovementComponent->Velocity.ToString()));
	}

	if (OtherActor != NULL)
	{
		// Если есть физ материал наносим урон и спавним виджет (Индиктор урона)
		if (Hit.PhysMaterial.Get())
		{
			MultiplayDamageByPhysMat(Hit.PhysMaterial.Get());
			UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this,
			                              UDamageType::StaticClass());

			SpawnWidgetDamageIndicator(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
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
