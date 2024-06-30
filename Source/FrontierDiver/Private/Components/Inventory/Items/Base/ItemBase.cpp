// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/Base/ItemBase.h"
#include "Components/Inventory/Items/WorldItems/WorldItemBase.h"
#include "Components/Inventory/InventoryComponent.h"

bool UItemBase::AddThisItemToInventory(UInventoryComponent* Inventory)
{
	return false;
}

bool UItemBase::RemoveThisItemFromInventory(UInventoryComponent* Inventory)
{
	return false;
}

bool UItemBase::DropThisItemFromInventory(UInventoryComponent* Inventory)
{
	return false;
}

