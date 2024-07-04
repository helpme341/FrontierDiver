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
				if (ItemTableRowInfo->ItemContainerType == EContainerType::ClothingOne ||
					ItemTableRowInfo->ItemContainerType == EContainerType::ClothingTwo)
				{
					Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<UItemBase*>(Item);
					Item->ThisItemID = 0;
					return true;
				}
				else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
				{
					TArray<UItemBase*>& ContainerArray = Inventory[ItemTableRowInfo->ItemContainerType].Item.TryGet<FContainerBase>()->Inventory;
					if (!ContainerArray.IsEmpty())
					{
						for (int32 Counter = 0; Counter < ContainerArray.Num(); Counter++)
						{
							if (!ContainerArray[Counter])
							{
								ContainerArray[Counter] = Item;
								Item->ThisItemID = Counter;
								return true;
							}
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
				if (ItemTableRowInfo->ItemContainerType == EContainerType::ClothingOne ||
					ItemTableRowInfo->ItemContainerType == EContainerType::ClothingTwo)
				{
					Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<UItemBase*>(nullptr);
					Item->ThisItemID = 99;
					if (DestroyItem) { Item->ConditionalBeginDestroy(); }
					return true;
				}
				else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
				{
					TArray<UItemBase*> ContainerArray = Inventory[ItemTableRowInfo->ItemContainerType].Item.TryGet<FContainerBase>()->Inventory;

					if (!ContainerArray.IsEmpty())
					{
						ContainerArray[Item->ThisItemID] = nullptr;
						Item->ThisItemID = 99;
						if (DestroyItem) { Item->ConditionalBeginDestroy(); }
						return true;
					}
				}
			}
		}
	}
	return false;
}

