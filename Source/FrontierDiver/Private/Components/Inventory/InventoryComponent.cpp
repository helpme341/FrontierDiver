// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"


bool UInventoryComponent::AddItemToInventory(UItemBase* Item)
{
	if (Item) { if (Item->AddThisItemToInventory(this)) { return true; } }
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
	if (Item) { if (Item->RemoveThisItemFromInventory(this, DestroyItem)) { return true; } }
	return false;
}

bool UInventoryComponent::PickupItemToInventory(AWorldItem* Item)
{
	if (Item)
	{
		UItemBase* NewItem = NewObject<UItemBase>(this, Item->ItemType);
		if (NewItem) { if (Cast<IPickupDropItemIF>(NewItem)->PickupItem(this, Item)) { return true; } }
	}
	return false;
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
	if (Item) { if (Cast<IPickupDropItemIF>(Item)->DropItem(this)) { return true; } }
	return false;
}

UDataTable* UInventoryComponent::FindDataTableByItemType(TSubclassOf<UItemBase> Item)
{
	if (DataTablesInfo.Contains(Item)) { return DataTablesInfo[Item]; }
	return nullptr;
}