// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JOYWAYTEST_API IHandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void GrabActor();

	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void ReleaseActor();

	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void TriggerPressed();

	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void TriggerReleased();

	UFUNCTION(Category="State", BlueprintCallable, BlueprintNativeEvent)
	void SetHandVisibleState(bool Visible);
};
