// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageIndicator.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "BulletProjectTile.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API ABulletProjectTile : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ABulletProjectTile();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleSystemHitEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADamageIndicator> DamageIndicator;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);
	void MultiplayDamageByPhysMat(UPhysicalMaterial* PhysMat);

	UFUNCTION()
	void DestroyProjectile();

	UFUNCTION()
	void SpawnHitEffectEmmiter(const FHitResult& Hit);

	UFUNCTION()
	void ProjectileStop(const FHitResult& ImpactResult);

	UFUNCTION()
	void SpawnWidgetDamageIndicator(FVector WorldLocation, FRotator WorldRotation);
};
