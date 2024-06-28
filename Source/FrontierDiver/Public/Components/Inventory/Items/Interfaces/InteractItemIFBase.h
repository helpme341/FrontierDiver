// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/Inventory/Items/Interfaces/ItemIFBase.h"
#include "InteractItemIFBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractItemIFBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IInteractItemIFBase : public IItemIFBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void TakeItem() = 0;

	virtual void DropItem() = 0;
};
