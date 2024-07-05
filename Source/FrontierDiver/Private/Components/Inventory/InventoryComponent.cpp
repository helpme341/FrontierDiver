// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"
#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"


bool UInventoryComponent::AddItemToInventory(UItemBase* Item)
{
    if (Item->bUseCustomAddThisItemToInventory)
    {
        if (Item) { if (Item->AddThisItemToInventory(this)) { return true; } }
        return false;
    }
    else
    {
        return BaseAddItemToInventory(Item);
    }
}

bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->bUseCustomRemoveThisItemFromInventory)
    {
        if (Item) { if (Item->RemoveThisItemFromInventory(this, DestroyItem)) { return true; } }
        return false;
    }
    else
    {
        return BaseRemoveItemFromInventory(Item, DestroyItem);
    }
}

bool UInventoryComponent::PickupItemToInventory(AWorldItem* Item)
{
	UItemBase* NewItem = NewObject<UItemBase>(this, Item->ItemType);

	if (IPickupDropItemIF* PickupDropItemIF = Cast<IPickupDropItemIF>(NewItem))
	{
		return PickupDropItemIF->PickupItem(this, Item);
	}
	else { NewItem->ConditionalBeginDestroy(); }
	return false;
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
	if (IPickupDropItemIF* PickupDropItemIF = Cast<IPickupDropItemIF>(Item))
	{
		return PickupDropItemIF->DropItem(this);
	}
	return false;
}

UDataTable* UInventoryComponent::FindDataTableByItemType(TSubclassOf<UItemBase> Item)
{
	if (DataTablesInfo.Contains(Item)) { return DataTablesInfo[Item]; }
	return nullptr;
}

// бызывие релизации функций придметов 
bool UInventoryComponent::BaseAddItemToInventory(UItemBase* Item)
{
	if (Item->ThisItemID == 99 && Item)
	{
		if (Item->FindDataTableByItemType(this))
		{
			FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
			if (ItemTableRowInfo && Inventory.Contains(ItemTableRowInfo->ItemContainerType))
			{
				if (!Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory.IsEmpty())
				{
					for (int32 Counter = 0; Counter < Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory.Num(); Counter++)
					{
						if (!Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Counter])
						{
							Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Counter] = Item;
							Item->ThisItemID = Counter;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool UInventoryComponent::BaseRemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
	if (Item->ThisItemID != 99 && Item)
	{
		if (Item->FindDataTableByItemType(this))
		{
			FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
			if (ItemTableRowInfo && Inventory.Contains(ItemTableRowInfo->ItemContainerType))
			{
				Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Item->ThisItemID] = nullptr;
				Item->ThisItemID = 99;
				if (DestroyItem) { Item->ConditionalBeginDestroy(); }
				return true;
			}
		}
	}
	return false;
}

