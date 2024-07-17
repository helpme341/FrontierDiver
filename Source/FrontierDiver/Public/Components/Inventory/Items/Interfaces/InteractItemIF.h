// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractItemIF.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractItemIF : public UInterface
{
	GENERATED_BODY()
};

class UInventoryComponent;

/**
 * 
 */ 
class FRONTIERDIVER_API IInteractItemIF : public IInterface
{
	GENERATED_BODY()

public:


	virtual void  MainInteract(UInventoryComponent* Inventory) {};

	virtual void FirstInteract(UInventoryComponent* Inventory) {};

	virtual void SecondInteract(UInventoryComponent* Inventory) {};

	virtual void ThirdInteract(UInventoryComponent* Inventory) {};
};
