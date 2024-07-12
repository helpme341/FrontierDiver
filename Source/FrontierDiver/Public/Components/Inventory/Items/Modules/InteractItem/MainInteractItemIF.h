// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/Inventory/Items/Modules/ItemIFBase.h"
#include "MainInteractItemIF.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainInteractItemIF : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IMainInteractItemIF : public IItemIFBase
{
	GENERATED_BODY()

public:

	virtual void MainInteract(class UInventoryComponent* Inventory) =0;
};
