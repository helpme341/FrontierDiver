// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/Modules/ItemIFBase.h"
#include "PickupDropItemIF.generated.h"

class AWorldItem;
class UInventoryComponent;



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupDropItemIF : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IPickupDropItemIF: public IItemIFBase
{
	GENERATED_BODY()
public:

	virtual bool PickupItem(UInventoryComponent* Inventory, AWorldItem* Item) =0;

	virtual bool DropItem(UInventoryComponent* Inventory) =0;
};
