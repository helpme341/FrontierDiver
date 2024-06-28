// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/Wear/Costumes/CostumeWearItemBase.h"
#include "Components/Inventory/InventoryComponent.h"

bool CostumeWearItemBase::AddThisItemToInventory(UInventoryComponent* Inventory)
{
	if (!Inventory->WearCostume)
	{
		Inventory->WearCostume = this;
		return true;
	}
    return false;
}

bool CostumeWearItemBase::RemoveThisItemFromInvenory(UInventoryComponent* Inventory)
{
	if (Inventory->WearCostume == this)
	{
		Inventory->WearCostume = nullptr;
		return true;
	}
	return false;
}