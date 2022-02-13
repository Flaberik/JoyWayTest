// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"


// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%f"),	Axis));

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("RotateInput", this, &ABaseCharacter::RotatePawn);
}

void ABaseCharacter::MoveForward(float Axis)
{
	ValidateAxisInput(&Axis);

	if (Axis != 0.0f)
	{
		const FVector Direction = FVector(Camera->GetForwardVector().X, Camera->GetForwardVector().Y, 0.0f);
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::MoveRight(float Axis)
{
	ValidateAxisInput(&Axis);

	if (Axis != 0.0f)
	{
		const FVector Direction = FVector(Camera->GetRightVector().X, Camera->GetRightVector().Y, 0.0f);
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::RotatePawn(float Axis)
{
	ValidateAxisInput(&Axis);
	if (Axis != 0.0f)
	{
		AddControllerYawInput(Axis);
	}
}

void ABaseCharacter::ValidateAxisInput(float* Axis)
{
	if (abs(*Axis) > DeathZoneDown)
	{
		if (*Axis > 0.0f)
		{
			if (*Axis > DeathZoneTop)
				*Axis = 1.0f;
		}
		else
		{
			if (abs(*Axis) > DeathZoneTop)
				*Axis = -1.0f;
		}
	}
	else
	{
		*Axis = 0.0f;
	}
}
