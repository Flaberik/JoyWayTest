// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class JOYWAYTEST_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category="Movement", VisibleAnywhere, BlueprintReadWrite)
	float DeathZoneDown = 0.3f;
	UPROPERTY(Category="Movement", VisibleAnywhere, BlueprintReadWrite)
	float DeathZoneTop = 0.7f;
	UPROPERTY(Category="Movement", VisibleAnywhere, BlueprintReadWrite)
	float RotationSpeed = 80.0f;

	void MoveForward( float Axis);
	void MoveRight( float Axis);
	void RotatePawn(float Axis);
	void ValidateAxisInput(float *Axis);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	USceneComponent* VROrigin;
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

};
