// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JOYWAYTEST_API IPickUpInterface
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void PickUp(USceneComponent* AttachTo);

	UFUNCTION(Category="Interaction", BlueprintCallable, BlueprintNativeEvent)
	void Drop(AActor* Hand);

	UFUNCTION(Category="Physics", BlueprintCallable, BlueprintNativeEvent)
	UPrimitiveComponent* GetPhysicsComponent();
};
