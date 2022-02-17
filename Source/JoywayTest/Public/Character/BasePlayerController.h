// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API ABasePlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PlayerClassToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* UnpossedPawn;
	
	virtual void RespawnPlayer_Implementation() override;
	virtual void OnUnPossess() override;
	
};
