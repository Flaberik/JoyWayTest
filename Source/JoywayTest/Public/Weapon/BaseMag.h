// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletProjectTile.h"

#include "SimplePickUpActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseMag.generated.h"

UCLASS()
class JOYWAYTEST_API ABaseMag : public ASimplePickUpActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseMag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SM_MagComponent;

	// Используется для пересечения с коллизией сокета у оружия 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxDetectMagSocket;

	// Мэш пустой обоймы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_MagEmptyState;

	// Мэш полной обоймы (не пустой)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_MagFullState;

	// Используемый тип пули
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABulletProjectTile> UseBullet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> GrabbedHand;

	// Вместительность обоймы
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxBulletAmount;

	// Оствшиеся патроны
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BulletAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PickUp_Implementation(USceneComponent* AttachTo) override;

	// Вызывавется когда игрок подбирает обойму
	// Обновляет статическую сетку на пустой или полный магазин
	void UpdateState();

};
