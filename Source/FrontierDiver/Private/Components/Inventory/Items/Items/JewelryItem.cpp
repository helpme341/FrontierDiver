// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/Items/JewelryItem.h"
#include "Components/Inventory/Items/WorldItems/JewelryWorldItem.h"

UJewelryItem::UJewelryItem()
{
	TemplateItem.Owner = this;
}

bool UJewelryItem::AddThisItemToInventory(UInventoryComponent* Inventory)
{
	return TemplateItem.AddThisItemToInventory(Inventory);
}

bool UJewelryItem::RemoveThisItemFromInventory(UInventoryComponent* Inventory)
{
	return TemplateItem.RemoveThisItemFromInventory(Inventory);
}

bool UJewelryItem::DropThisItemFromInventory(UInventoryComponent* Inventory)
{
	return TemplateItem.DropThisItem(Inventory);
}
