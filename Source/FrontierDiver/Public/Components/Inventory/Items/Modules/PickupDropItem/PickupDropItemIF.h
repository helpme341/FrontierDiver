// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/Modules/ItemIFBase.h"

class AWorldItem;
class UInventoryComponent;

/**
 * 
 */
class FRONTIERDIVER_API IPickupDropItemIF: public IItemIFBase
{
public:

	virtual bool PickupItem(UInventoryComponent* Inventory, AWorldItem* Item) =0;

	virtual bool DropItem(UInventoryComponent* Inventory) =0;
};
