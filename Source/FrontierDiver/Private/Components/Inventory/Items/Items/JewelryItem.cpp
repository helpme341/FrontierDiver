// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/Items/JewelryItem.h"
#include "Components/Inventory/Items/WorldItems/JewelryWorldItem.h"

bool UJewelryItem::AddThisItemToInventory(UInventoryComponent* Inventory)
{
	return AddItemToInventory(Inventory);
}

bool UJewelryItem::RemoveThisItemFromInventory(UInventoryComponent* Inventory)
{
	return RemoveItemFromInventory(Inventory);
}

bool UJewelryItem::DropThisItemFromInventory(UInventoryComponent* Inventory)
{
	return DropItem(Inventory);
}
