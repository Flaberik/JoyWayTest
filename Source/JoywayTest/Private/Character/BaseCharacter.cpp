// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "HandInterface.h"
#include "InputBehavior.h"


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

	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteractionComponent->SetupAttachment(VROrigin);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnHands();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePlayerLocationByCamera();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("RotateInput", this, &ABaseCharacter::RotatePawn);

	PlayerInputComponent->BindAction("TriggerRight", IE_Pressed, this, &ABaseCharacter::TriggerRightPressed);
	PlayerInputComponent->BindAction("TriggerRight", IE_Released, this, &ABaseCharacter::TriggerRightReleased);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Pressed, this, &ABaseCharacter::TriggerLeftPressed);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Released, this, &ABaseCharacter::TriggerLeftReleased);

	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &ABaseCharacter::GrabLeft);
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &ABaseCharacter::GrabRight);

}

void ABaseCharacter::MoveForward(float Axis)
{
	ValidateAxisInput(Axis);

	if (Axis != 0.0f)
	{
		const FVector Direction = FVector(Camera->GetForwardVector().X, Camera->GetForwardVector().Y, 0.0f);
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::MoveRight(float Axis)
{
	ValidateAxisInput(Axis);

	if (Axis != 0.0f)
	{
		const FVector Direction = FVector(Camera->GetRightVector().X, Camera->GetRightVector().Y, 0.0f);
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::RotatePawn(float Axis)
{
	ValidateAxisInput(Axis);
	if (Axis != 0.0f)
	{
		AddControllerYawInput(Axis);
	}
}

void ABaseCharacter::ValidateAxisInput(float& Axis)
{
	if (abs(Axis) > DeathZoneDown)
	{
		if (Axis > 0.0f)
		{
			if (Axis > DeathZoneTop)
				Axis = 1.0f;
		}
		else
		{
			if (abs(Axis) > DeathZoneTop)
				Axis = -1.0f;
		}
	}
	else
	{
		Axis = 0.0f;
	}
}

void ABaseCharacter::SpawnHands()
{
	// Right hand 
	RightHand = GetWorld()->SpawnActorDeferred<AHand>(HandClass, FTransform::Identity, this, this);
	RightHand->HandSide = EHandSide::Right;
	RightHand->FinishSpawning(FTransform::Identity);
	RightHand->AttachToComponent(VROrigin, FAttachmentTransformRules::KeepRelativeTransform);
	
	WidgetInteractionComponent->AttachToComponent(RightHand->MotionControllerComponent,
	                                              FAttachmentTransformRules::KeepRelativeTransform);

	// Left Hand
	LeftHand = GetWorld()->SpawnActorDeferred<AHand>(HandClass, FTransform::Identity, this, this);
	LeftHand->HandSide = EHandSide::Left;
	LeftHand->FinishSpawning(FTransform::Identity);
	LeftHand->AttachToComponent(VROrigin, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABaseCharacter::TriggerRightPressed()
{
	if(RightHand != nullptr)
	{
		Cast<IHandInterface>(RightHand)->Execute_TriggerPressed(RightHand);
	}

	if(WidgetInteractionComponent->bEnableHitTesting)
	{
		WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void ABaseCharacter::TriggerRightReleased()
{
	if(RightHand != nullptr)
	{
		Cast<IHandInterface>(RightHand)->Execute_TriggerReleased(RightHand);
	}

	WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);
	
}

void ABaseCharacter::TriggerLeftPressed()
{
	if(LeftHand != nullptr)
	{
		Cast<IHandInterface>(LeftHand)->Execute_TriggerPressed(LeftHand);
	}
}

void ABaseCharacter::TriggerLeftReleased()
{
	if(LeftHand != nullptr)
	{
		Cast<IHandInterface>(LeftHand)->Execute_TriggerReleased(LeftHand);
	}
}

void ABaseCharacter::GrabRight()
{
	if(RightHand != nullptr)
	{
		Cast<IHandInterface>(RightHand)->Execute_GrabActor(RightHand);
	}
}

void ABaseCharacter::GrabLeft()
{
	if(LeftHand != nullptr)
	{
		Cast<IHandInterface>(LeftHand)->Execute_GrabActor(LeftHand);
	}
}

void ABaseCharacter::UpdatePlayerLocationByCamera()
{
	FVector VROriginLocation = VROrigin->GetComponentLocation();

	FVector NewPlayerLocation = Camera->GetComponentLocation();
	NewPlayerLocation.Z = GetActorLocation().Z;

	SetActorLocation(NewPlayerLocation, true);
	VROrigin->SetWorldLocation(VROriginLocation);
}

void ABaseCharacter::SetEnabledWidgetInteraction_Implementation(bool Enabled)
{
	IPlayerInterface::SetEnabledWidgetInteraction_Implementation(Enabled);
	WidgetInteractionComponent->bEnableHitTesting = Enabled;
}

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();
	
	LeftHand->Destroy();
	RightHand->Destroy();
}
