// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UInventoryComponent;

/**
 * 
 */
class FRONTIERDIVER_API ItemBase
{
public:

	virtual ~ItemBase() {}

	virtual bool AddThisItemToInventory(UInventoryComponent* Inventory);

	virtual bool RemoveThisItemFromInvenory(UInventoryComponent* Inventory);
};
