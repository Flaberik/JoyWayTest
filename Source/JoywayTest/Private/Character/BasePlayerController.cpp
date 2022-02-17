// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BasePlayerController.h"

#include "Character/BaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ABasePlayerController::RespawnPlayer_Implementation()
{
	IPlayerControllerInterface::RespawnPlayer_Implementation();

	APlayerStart* FindPlayerStart = Cast<APlayerStart>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	if(FindPlayerStart == nullptr)
		return;
	
	APawn* SpawnedPawn = Cast<APawn>(
		GetWorld()->SpawnActor(PlayerClassToSpawn, &FindPlayerStart->GetActorTransform()));

	UnpossedPawn = GetPawn();

	Possess(SpawnedPawn);
}

void ABasePlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (UnpossedPawn != nullptr)
		UnpossedPawn->Destroy();
}
