// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/InventoryComponent.h"

bool UItemBase::FindDataTableByItemType(UInventoryComponent* Inventory)
{
    return false;
}

FItemTableRowInfoBase* UItemBase::GetItemStaticInfo()
{
    return nullptr;
}

bool UItemBase::AddThisItemToInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID == 99)
    {
        if (FindDataTableByItemType(Inventory))
        {
            FItemTableRowInfoBase* ItemTableRowInfo = GetItemStaticInfo();
            if (ItemTableRowInfo && Inventory->Inventory.Contains(ItemTableRowInfo->ItemContainerType))
            {
                if (ItemTableRowInfo->ItemContainerType == EContainerType::ClothingOne ||
                    ItemTableRowInfo->ItemContainerType == EContainerType::ClothingTwo)
                {
                    Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<UItemBase*>(this);
                    ThisItemID = 0;
                    return true;
                }
                else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
                {
                    TArray<UItemBase*>& ContainerArray = Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.TryGet<FContainerBase>()->Inventory;
                    if (!ContainerArray.IsEmpty())
                    {
                        for (int32 Counter = 0; Counter < ContainerArray.Num(); Counter++)
                        {
                            if (!ContainerArray[Counter])
                            {
                                ContainerArray[Counter] = this;
                                ThisItemID = Counter;
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

bool UItemBase::RemoveThisItemFromInventory(UInventoryComponent* Inventory, bool DestroyItem)
{
    if (ThisItemID != 99)
    {
        if (FindDataTableByItemType(Inventory))
        {
            FItemTableRowInfoBase* ItemTableRowInfo = GetItemStaticInfo();
            if (ItemTableRowInfo && Inventory->Inventory.Contains(ItemTableRowInfo->ItemContainerType))
            {
                if (ItemTableRowInfo->ItemContainerType == EContainerType::ClothingOne ||
                    ItemTableRowInfo->ItemContainerType == EContainerType::ClothingTwo)
                {
                    Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<UItemBase*>(nullptr);
                    ThisItemID = 99;
                    if (DestroyItem) { this->ConditionalBeginDestroy(); }
                    return true;
                }
                else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
                {
                    TArray<UItemBase*> ContainerArray = Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.TryGet<FContainerBase>()->Inventory;

                    if (!ContainerArray.IsEmpty())
                    {
                        ContainerArray[ThisItemID] = nullptr;
                        ThisItemID = 99;
                        if (DestroyItem) { this->ConditionalBeginDestroy(); }
                        return true;
                    }
                }
            }
        }
    }
    return false;
}