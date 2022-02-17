// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeaponGun.h"

#include "HandInterface.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseWeaponGun::ABaseWeaponGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Gun"));
	RootComponent = SM_Gun;


	SM_PrimaryHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_PrimaryHand"));
	SM_PrimaryHand->SetupAttachment(RootComponent);

	SM_SecondaryHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_SecondaryHand"));
	SM_SecondaryHand->SetupAttachment(RootComponent);

	ShootInitialPoint = CreateDefaultSubobject<USceneComponent>("ShootInitialPoint");
	ShootInitialPoint->SetupAttachment(RootComponent);

	ShootDirectionArrow = CreateDefaultSubobject<UArrowComponent>("ShootDirectionArrow");
	ShootDirectionArrow->SetupAttachment(ShootInitialPoint);

	MagDetector = CreateDefaultSubobject<UBoxComponent>("MagDetector");
	MagDetector->SetupAttachment(RootComponent);

	MagDetector->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeaponGun::MagDetectorOnOverlapBegin);
	MagDetector->OnComponentEndOverlap.AddDynamic(this, &ABaseWeaponGun::MagDetectorOnOverlapEnd);

	MagPointSocket = CreateDefaultSubobject<USceneComponent>("MagPointSocket");

	MagPointSocket->SetupAttachment(MagDetector);

	AmmoDisplay = CreateDefaultSubobject<UTextRenderComponent>(TEXT("AmmoDisp"));
	AmmoDisplay->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseWeaponGun::BeginPlay()
{
	Super::BeginPlay();

	UpdateAmmoDisplay();
}

// Called every frame
void ABaseWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateGunByHands();
}

void ABaseWeaponGun::Shoot()
{
	if (!CanShoot())
	{
		GetWorldTimerManager().ClearTimer(ShootTimerHandle);
		return;
	}
	
	SpawnProjectile();
	SpawnParticleFireEffect();
	
	if (MagActor)
		MagActor->BulletAmount -= 1;

	UpdateAmmoDisplay();
}

void ABaseWeaponGun::SpawnProjectile()
{
	if (MagActor == nullptr)
		return;

	FVector Location = ShootInitialPoint->GetComponentLocation();
	FRotator Rotation = ShootInitialPoint->GetComponentRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	GetWorld()->SpawnActor<ABulletProjectTile>(MagActor->UseBullet, Location, Rotation, SpawnParameters);
}

void ABaseWeaponGun::SpawnParticleFireEffect()
{
	if (ParticleSystemFireEffect == nullptr)
		return;

	FTransform SpawnTransform = FTransform(ShootInitialPoint->GetComponentRotation(),
	                                       ShootInitialPoint->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemFireEffect, SpawnTransform);
}

void ABaseWeaponGun::UpdateAmmoDisplay()
{
	int BulletAmount = 0;
	int MaxBulletAmount = 99;
	FColor DisplayColor = LowAmmoDisplayColor;

	if (MagActor != nullptr)
	{
		BulletAmount = MagActor->BulletAmount;
		MaxBulletAmount = MagActor->MaxBulletAmount;
	}

	if (UKismetMathLibrary::InRange_IntInt(BulletAmount, MaxBulletAmount - MaxBulletAmount / 3, MaxBulletAmount))
	{
		DisplayColor = ManyAmmoDisplayColor;
	}
	else if (UKismetMathLibrary::InRange_IntInt(BulletAmount, MaxBulletAmount - MaxBulletAmount / 3 * 2,
	                                            MaxBulletAmount - MaxBulletAmount / 3))
	{
		DisplayColor = HalfAmmoDisplayColor;
	}

	if (BulletAmount <= 0)
	{
		AmmoDisplay->Text = FText::FromString("-");
	}
	else
	{
		AmmoDisplay->Text = FText::FromString(FString::FromInt(BulletAmount));
	}

	AmmoDisplay->SetTextRenderColor(DisplayColor);
}

bool ABaseWeaponGun::CanShoot()
{
	if (MagActor != nullptr && MagActor->BulletAmount > 0 && PrimaryHand != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ABaseWeaponGun::TryGrabPrimaryHand(USceneComponent* AttachTo)
{
	if (AttachTo == nullptr)
		return false;
	if (PrimaryHand != nullptr)
		return false;

	PrimaryHand = AttachTo->GetOwner();
	PrimaryHandAttachComponent = AttachTo;

	SM_Gun->SetSimulatePhysics(false);

	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	K2_GetRootComponent()->AttachToComponent(AttachTo, AttachmentTransformRules);
	if (PrimaryHand->GetClass()->ImplementsInterface(UHandInterface::StaticClass()))
	{
		Cast<IHandInterface>(PrimaryHand)->Execute_SetHandVisibleState(PrimaryHand, false);
	}

	SM_PrimaryHand->SetVisibility(true);

	return true;
}

void ABaseWeaponGun::TryGrabSecondaryHand(USceneComponent* AttachTo)
{
	if (!AttachTo)
		return;
	if (!PrimaryHand)
		return;

	SecondaryHand = AttachTo->GetOwner();
	SecondaryHandAttachComponent = AttachTo;

	if (SecondaryHand->GetClass()->ImplementsInterface(UHandInterface::StaticClass()))
	{
		Cast<IHandInterface>(SecondaryHand)->Execute_SetHandVisibleState(SecondaryHand, false);
	}

	SM_SecondaryHand->SetVisibility(true);
}


void ABaseWeaponGun::PickUp_Implementation(USceneComponent* AttachTo)
{
	IPickUpInterface::PickUp_Implementation(AttachTo);

	if (TryGrabPrimaryHand(AttachTo))
		return;

	TryGrabSecondaryHand(AttachTo);
}

// Вызыввается когда игрок нажимает кнопку стрельбы (Сработает если это главная рука)
void ABaseWeaponGun::StartShoot_Implementation(AActor* Hand)
{
	IGunInterface::StartShoot_Implementation(Hand);

	if (!(Hand && Hand == PrimaryHand))
		return;

	// Первый Выстрел, потом стреляет автоматически если кнопка зажата
	Shoot();
	
	float ShootTimerDelay = 60.0f / FireRateInMinute;
	GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &ABaseWeaponGun::Shoot, ShootTimerDelay, true,ShootTimerDelay);
}

// Вызыввается когда игрок отпускает кнопку стрельбы (Сработает если это главная рука)
void ABaseWeaponGun::EndShoot_Implementation(AActor* Hand)
{
	IGunInterface::EndShoot_Implementation(Hand);

	if (!(Hand && Hand == PrimaryHand))
		return;

	IGunInterface::EndShoot_Implementation(Hand);
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ABaseWeaponGun::Drop_Implementation(AActor* Hand)
{
	IPickUpInterface::Drop_Implementation(Hand);
	
	if (Hand == PrimaryHand)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		TryReleasePrimaryHand();
		TryReleaseSecondaryHand();

		SM_Gun->SetSimulatePhysics(true);
	}
	else if (Hand == SecondaryHand)
	{
		TryReleaseSecondaryHand();
	}
}

UPrimitiveComponent* ABaseWeaponGun::GetPhysicsComponent_Implementation()
{
	return SM_Gun;
}

void ABaseWeaponGun::TryReleasePrimaryHand()
{
	if (PrimaryHand == nullptr)
		return;
	if (!PrimaryHand->GetClass()->ImplementsInterface(UHandInterface::StaticClass()))
		return;

	Cast<IHandInterface>(PrimaryHand)->Execute_SetHandVisibleState(PrimaryHand, true);
	Cast<IHandInterface>(PrimaryHand)->Execute_ReleaseActor(PrimaryHand);

	PrimaryHand = nullptr;
	PrimaryHandAttachComponent = nullptr;
	SM_PrimaryHand->SetVisibility(false);
}

void ABaseWeaponGun::TryReleaseSecondaryHand()
{
	if (SecondaryHand == nullptr)
		return;
	if (!SecondaryHand->GetClass()->ImplementsInterface(UHandInterface::StaticClass()))
		return;

	Cast<IHandInterface>(SecondaryHand)->Execute_SetHandVisibleState(SecondaryHand, true);
	Cast<IHandInterface>(SecondaryHand)->Execute_ReleaseActor(SecondaryHand);

	SecondaryHand = nullptr;
	SecondaryHandAttachComponent = nullptr;
	SM_SecondaryHand->SetVisibility(false);
}

void ABaseWeaponGun::RotateGunByHands()
{
	if (PrimaryHandAttachComponent && SecondaryHandAttachComponent)
	{
		FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(
			PrimaryHandAttachComponent->GetComponentLocation(),
			SecondaryHandAttachComponent->GetComponentLocation());

		LookRotator.Roll = PrimaryHandAttachComponent->GetComponentRotation().Roll;

		SM_Gun->SetWorldRotation(LookRotator);
	}
}


void ABaseWeaponGun::MagDetectorOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	if (MagActor != nullptr)
		return;

	if (OtherActor->GetClass() != AvailableMag.Get())
		return;

	if (!OtherActor->GetClass()->ImplementsInterface(UPickUpInterface::StaticClass()))
		return;

	OtherActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OtherActor->AttachToComponent(MagPointSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	MagActor = Cast<ABaseMag>(OtherActor);

	if (MagActor->GetOwner() && MagActor->GetOwner()->GetClass()->ImplementsInterface(UHandInterface::StaticClass()))
	{
		Cast<IHandInterface>(MagActor->GetOwner())->Execute_ReleaseActor(MagActor->GetOwner());
	}

	UpdateAmmoDisplay();
}

void ABaseWeaponGun::MagDetectorOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == MagActor)
	{
		MagActor = nullptr;
		UpdateAmmoDisplay();
	}
}
