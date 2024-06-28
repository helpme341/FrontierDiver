// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/Bases/WearItemBase.h"

/**
 * 
 */
class FRONTIERDIVER_API FlipperWearItemBase
{
public:
	FlipperWearItemBase();
	virtual ~FlipperWearItemBase();


	virtual bool AddThisItemToInventory(UInventoryComponent* Inventory) override;

	virtual bool RemoveThisItemFromInvenory(UInventoryComponent* Inventory) override;
};

