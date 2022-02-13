// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageIndicator.generated.h"

UCLASS()
class JOYWAYTEST_API ADamageIndicator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageIndicator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	float Damage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	float LifeTime = 3.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
