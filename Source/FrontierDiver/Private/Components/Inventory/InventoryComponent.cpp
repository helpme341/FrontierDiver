// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/Base/ItemBase.h"
#include "Components/Inventory/Items/WorldItems/WorldItemBase.h"


bool UInventoryComponent::AddItemToInventory(UItemBase* Item)
{
	if (Item) { if (Item->AddThisItemToInventory(this)) { return true; } }
	return false;
}

bool UInventoryComponent::AddItemToInventory(AWorldItemBase* Item)
{
	if (Item) { if (Item->AddThisItemToInventory(this)) { return true; } }
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item)
{
	if (Item) { if (Item->RemoveThisItemFromInventory(this)) { return true; } }
	return false;
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
	if (Item) { if (Item->DropThisItemFromInventory(this)) { return true; } }
	return false;
}

UDataTable* UInventoryComponent::FindDataTableByStructType(TSubclassOf<UItemBase> Item)
{
	if (DataTablesInfo.Contains(Item))
	{
		return DataTablesInfo[Item];
	}
	return nullptr;
}