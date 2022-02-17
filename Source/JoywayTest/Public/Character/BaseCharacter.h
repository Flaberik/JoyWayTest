// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class JOYWAYTEST_API ABaseCharacter : public ACharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category="Components", VisibleAnywhere, BlueprintReadWrite)
	UWidgetInteractionComponent* WidgetInteractionComponent;
	UPROPERTY(Category="Components", VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* VROrigin;
	UPROPERTY(Category="Components", VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(Category="Movement", EditAnywhere, BlueprintReadWrite)
	float DeathZoneDown = 0.3f;
	UPROPERTY(Category="Movement", EditAnywhere, BlueprintReadWrite)
	float DeathZoneTop = 0.7f;
	UPROPERTY(Category="Movement", EditAnywhere, BlueprintReadWrite)
	float RotationSpeed = 80.0f;

	UPROPERTY(Category="Hands", EditAnywhere, BlueprintReadWrite)
	AHand* RightHand;
	UPROPERTY(Category="Hands", EditAnywhere, BlueprintReadWrite)
	AHand* LeftHand;
	
	UPROPERTY(Category="Hands", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHand> HandClass;
	
	//Вызывается в тике. Используется что бы игрок не мог выйти из капсулы
	void UpdatePlayerLocationByCamera();

	virtual void SetEnabledWidgetInteraction_Implementation(bool Enabled) override;

	virtual void Destroyed() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward( float Axis);
	UFUNCTION()
	void MoveRight( float Axis);
	UFUNCTION()
	void RotatePawn(float Axis);
	UFUNCTION()
	void ValidateAxisInput(float &Axis);
	UFUNCTION()
	void SpawnHands();

	UFUNCTION()
	void TriggerRightPressed();
	UFUNCTION()
	void TriggerRightReleased();

	UFUNCTION()
	void TriggerLeftPressed();
	UFUNCTION()
	void TriggerLeftReleased();

	UFUNCTION()
	void GrabRight();
	UFUNCTION()
	void GrabLeft();
	
	
};
