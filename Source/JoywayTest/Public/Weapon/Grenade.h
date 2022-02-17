// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplePickUpActor.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AGrenade : public ASimplePickUpActor
{
	GENERATED_BODY()
	
public:
	AGrenade();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* SM_Grenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	float DelayBeforeExplosion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	float RadialDamageRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	UParticleSystem* ExplosionEffect;

	UFUNCTION(BlueprintCallable)
	void BlowUp();

	FTimerHandle BlowUpTimer;
	
	virtual void Drop_Implementation(AActor* Hand) override;
	virtual void PickUp_Implementation(USceneComponent* AttachTo) override;
};
