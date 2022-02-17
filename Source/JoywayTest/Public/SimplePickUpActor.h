// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpInterface.h"
#include "GameFramework/Actor.h"
#include "SimplePickUpActor.generated.h"

UCLASS()
class JOYWAYTEST_API ASimplePickUpActor : public AActor, public IPickUpInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimplePickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/**
	 * @brief Мэшка которой будет включатся и выключатся физика
	 */
	UStaticMeshComponent* PickUpMesh;
	AActor* GrabbedHand;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void PickUp_Implementation(USceneComponent* AttachTo) override;
	virtual void Drop_Implementation(AActor* Hand) override;
	virtual UPrimitiveComponent* GetPhysicsComponent_Implementation() override;
	
};
