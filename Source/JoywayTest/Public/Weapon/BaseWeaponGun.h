// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <VertexFormat.h>

#include "CoreMinimal.h"
#include "BaseMag.h"
#include "GunInterface.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponGun.generated.h"

UCLASS()
class JOYWAYTEST_API ABaseWeaponGun : public AActor, public IGunInterface, public IPickUpInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeaponGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SM_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SM_PrimaryHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SM_SecondaryHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* ShootInitialPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ShootDirectionArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* MagPointSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* MagDetector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* AmmoDisplay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FireRateInMinute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor ManyAmmoDisplayColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor HalfAmmoDisplayColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor LowAmmoDisplayColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleSystemFireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseMag> AvailableMag;

	UPROPERTY(BlueprintReadWrite)
	AActor* PrimaryHand;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* PrimaryHandAttachComponent;

	UPROPERTY(BlueprintReadWrite)
	AActor* SecondaryHand;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* SecondaryHandAttachComponent;

	UPROPERTY(BlueprintReadWrite)
	ABaseMag* MagActor;

	virtual void StartShoot_Implementation(AActor* Hand) override;
	virtual void EndShoot_Implementation(AActor* Hand) override;

	virtual void PickUp_Implementation(USceneComponent* AttachTo) override;
	virtual void Drop_Implementation(AActor* Hand) override;

	virtual UPrimitiveComponent* GetPhysicsComponent_Implementation() override;

	FTimerHandle ShootTimerHandle;

	UFUNCTION()
	bool CanShoot();

	UFUNCTION()
	void Shoot();
	
	UFUNCTION()
	void SpawnProjectile();
	
	UFUNCTION(BlueprintCallable)
	void UpdateAmmoDisplay();
	
	UFUNCTION()
	bool TryGrabPrimaryHand(USceneComponent* AttachTo);
	
	UFUNCTION()
	void TryGrabSecondaryHand(USceneComponent* AttachTo);
	
	UFUNCTION()
	void TryReleasePrimaryHand();
	
	UFUNCTION()
	void TryReleaseSecondaryHand();
	
	UFUNCTION()
	void RotateGunByHands();
	
	UFUNCTION()
	void SpawnParticleFireEffect();
	
	UFUNCTION()
	void MagDetectorOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);
	
	UFUNCTION()
	void MagDetectorOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
