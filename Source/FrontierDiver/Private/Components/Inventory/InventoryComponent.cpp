// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Character/FrontierDiverCharacter.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

bool UInventoryComponent::AddItemToInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->bUseCustomAddThisItemToInventory)
    {
        if (Item && Item->AddThisItemToInventory(this)) { return true; }
        UE_LOG(LogInventoryComponent, Warning, TEXT("Custom AddThisItemToInventory failed or Item is nullptr"));
        return false;
    }
    else
    {
        return BaseAddItemToInventory(Item, DestroyItem);
    }
}


bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->bUseCustomRemoveThisItemFromInventory)
    {
        if (Item) { if (Item->RemoveThisItemFromInventory(this, DestroyItem)) { return true; } }
        UE_LOG(LogInventoryComponent, Warning, TEXT("Custom RemoveThisItemFromInventory failed or Item is nullptr"));
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
    if (NewItem)
    {
        IPickupDropItemIF* PickupDropItemIF = Cast<IPickupDropItemIF>(NewItem);
        if (PickupDropItemIF)
        {
            if (PickupDropItemIF->PickupItem(this, Item))
            {
                if (NewItem->GetItemDynamicInfo()->QuantityItems == 0)
                {
                    NewItem->ConditionalBeginDestroy();
                }
                return true;
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to cast NewItem to IPickupDropItemIF or NewItem is nullptr"));
        }
        NewItem->ConditionalBeginDestroy();
    }
    else
    {
        UE_LOG(LogInventoryComponent, Error, TEXT("Failed to create NewItem"));
    }
    return false;
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
    if (IPickupDropItemIF* PickupDropItemIF = Cast<IPickupDropItemIF>(Item))
    {
        if (PickupDropItemIF->DropItem(this))
        {
            if (Item->GetItemDynamicInfo()->QuantityItems == 0) { Item->ConditionalBeginDestroy(); }
            return true;
        }
    }
    UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to cast Item to IPickupDropItemIF or Item is nullptr"));
    return false;
}

bool UInventoryComponent::TakeItemToHandsByID(int32& ID)
{
    return false;
}

bool UInventoryComponent::RemoveItemFromHands()
{
    return false;
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
            else
            {
                UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to create InventoryWidget"));
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("PlayerController not found"));
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("InventoryWidgetClass is nullptr"));
    }
}

bool UInventoryComponent::BaseAddItemToInventory(UItemBase* Item, bool DestroyItem)
{
    if (!Item || !Item->IsValidLowLevel() || Item->ThisItemID != 99)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or ThisItemID is not 99"));
        return false;
    }
    if (Item->FindDataTableByItemType())
    {
        FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
        if (ItemTableRowInfo && Inventory.Contains(ItemTableRowInfo->ItemContainerType))
        {
            TArray<UItemBase*>& ContainerInventory = Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory;
            if (!ContainerInventory.IsEmpty())
            {
                bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;
                FItemDynamicInfoBase* ItemDynamicInfo = Item->GetItemDynamicInfo();

                if (ItemDynamicInfo && ItemDynamicInfo->QuantityItems != 0)
                {
                    for (int32 Counter = 0; Counter < ContainerInventory.Num(); Counter++)
                    {
                        UItemBase* ItemOnInspection = ContainerInventory[Counter];

                        if (bCheckQuantity && ItemOnInspection)
                        {
                            FItemDynamicInfoBase* InspectionDynamicInfo = ItemOnInspection->GetItemDynamicInfo();

                            if (InspectionDynamicInfo && InspectionDynamicInfo->ItemTypeName == ItemDynamicInfo->ItemTypeName)
                            {
                                if (InspectionDynamicInfo->QuantityItems + ItemDynamicInfo->QuantityItems <= Item->GetItemStaticInfo()->MaxQuantityItemsInSlot)
                                {
                                    UE_LOG(LogInventoryComponent, Warning, TEXT("Adding items: %d + %d"), InspectionDynamicInfo->QuantityItems, ItemDynamicInfo->QuantityItems);
                                    InspectionDynamicInfo->QuantityItems += ItemDynamicInfo->QuantityItems;
                                    ItemDynamicInfo->QuantityItems = 0;
                                    InventoryWidget->UpdateWidgetByItem(ItemOnInspection, false);
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
                else
                {
                    UE_LOG(LogInventoryComponent, Warning, TEXT("ItemDynamicInfo is nullptr"));
                }
            }
            else
            {
                UE_LOG(LogInventoryComponent, Warning, TEXT("ContainerInventory is empty"));
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("ItemTableRowInfo is nullptr or ItemContainerType not found in Inventory"));
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
    }
    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
    return false;
}

bool UInventoryComponent::BaseRemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->ThisItemID != 99 && Item)
    {
        if (Item->FindDataTableByItemType())
        {
            FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
            FItemDynamicInfoBase* ItemDynamicInfo = Item->GetItemDynamicInfo();
            if (ItemTableRowInfo && ItemDynamicInfo)
            {
                if (ItemTableRowInfo->MaxQuantityItemsInSlot > 1 && ItemDynamicInfo->QuantityItems - 1 >= 1)
                {
                    ItemDynamicInfo->QuantityItems--;
                    InventoryWidget->UpdateWidgetByItem(Item, false);
                    return true;
                }
                else if (Inventory.Contains(ItemTableRowInfo->ItemContainerType))
                {
                    Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Item->ThisItemID] = nullptr;
                    Item->GetItemDynamicInfo()->QuantityItems = 0;
                    InventoryWidget->UpdateWidgetByItem(Item, true);
                    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
                    else { Item->ThisItemID = 99; }
                    return true;
                }
            }
            else
            {
                UE_LOG(LogInventoryComponent, Warning, TEXT("ItemTableRowInfo or ItemDynamicInfo is nullptr"));
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or ThisItemID is 99"));
    }
    return false;
}
