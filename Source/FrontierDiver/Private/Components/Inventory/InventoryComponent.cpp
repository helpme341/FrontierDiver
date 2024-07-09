// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
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

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);

			if (InventoryWidget)
			{
				InventoryWidget->InventoryComponent = this;
				InventoryWidget->LoadWidgestSlots();
				InventoryWidget->CreateWidgets();
				InventoryWidget->UpdateAllWidgets();
				InventoryWidget->AddToViewport(0);
			}
		}
	}
}

// бызывие релизации функций придметов 
bool UInventoryComponent::BaseAddItemToInventory(UItemBase* Item)
{
    if (!Item || Item->ThisItemID != 99)
    {
        return false;
    }

    if (Item->FindDataTableByItemType(this))
    {
        FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
        if (ItemTableRowInfo && Inventory.Contains(ItemTableRowInfo->ItemContainerType))
        {
            TArray<UItemBase*>& ContainerInventory = Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory;
            if (!ContainerInventory.IsEmpty())
            {
                bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;
                FItemDynamicInfoBase* ItemDynamicInfo = Item->GetItemDynamicInfo();

                for (int32 Counter = 0; Counter < ContainerInventory.Num(); Counter++)
                {
                    UItemBase* ItemOnInspection = ContainerInventory[Counter];

                    if (bCheckQuantity && ItemOnInspection)
                    {
                        FItemDynamicInfoBase* InspectionDynamicInfo = ItemOnInspection->GetItemDynamicInfo();

                        if (InspectionDynamicInfo->ItemTypeName == ItemDynamicInfo->ItemTypeName)
                        {
                            if (InspectionDynamicInfo->QuantityItems + ItemDynamicInfo->QuantityItems <= Item->GetItemStaticInfo()->MaxQuantityItemsInSlot)
                            {
                                InspectionDynamicInfo->QuantityItems += ItemDynamicInfo->QuantityItems;
                                InventoryWidget->UpdateWidgetByItem(ItemOnInspection, false);
                                Item->ConditionalBeginDestroy();
                                return true;
                            }
                        }
                    }

                    if (!ItemOnInspection)
                    {
                        ContainerInventory[Counter] = Item;
                        Item->ThisItemID = Counter;
                        InventoryWidget->UpdateWidgetByItem(Item, false);
                        return true;
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
            FItemDynamicInfoBase* ItemDynamicInfo = Item->GetItemDynamicInfo();
            if (ItemTableRowInfo && ItemDynamicInfo)
            {

                if (ItemTableRowInfo->MaxQuantityItemsInSlot > 1 && (ItemDynamicInfo->QuantityItems - 1) > 1)
                {
                    ItemDynamicInfo->QuantityItems--;
                    InventoryWidget->UpdateWidgetByItem(Item, false);
                }
                else if (Inventory.Contains(ItemTableRowInfo->ItemContainerType))
                {
                    Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Item->ThisItemID] = nullptr;
                    InventoryWidget->UpdateWidgetByItem(Item, true);
                    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
                    else { Item->ThisItemID = 99; }
                    return true;
                }
            }

		}
	}
	return false;
}

