// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JOYWAYTEST_API IPlayerInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void SetEnabledWidgetInteraction(bool Enabled);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
