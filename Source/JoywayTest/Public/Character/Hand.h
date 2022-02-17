// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "Hand.generated.h"

UENUM(BlueprintType)
enum class EHandSide : uint8
{
	Right = 0 UMETA(DisplayName = "Right"),
	Left = 1 UMETA(DisplayName = "Left"),
};

UCLASS()
class JOYWAYTEST_API AHand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHand();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UMotionControllerComponent* MotionControllerComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHandSide HandSide;
};
