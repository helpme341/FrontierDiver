// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponent.h"

class InventoryItemClassBase;
class AWorldItemBase;


/**
 *  OWT = Owner Type (class)
 *  DT = Owner Dynamic Info Type (struct)
 *  DTT = Owner Table Row Type (struct)
 *  WT = Owner World Actor Type (class)
 */
template<typename OWT, typename DT, typename TRT, typename WT = AWorldItemBase>
class FRONTIERDIVER_API TTemplateItem
{
public:
    DT ItemDynamicInfo;
    TRT* ItemTableRowInfo;
    OWT* Owner;

    bool AddItemToInventory(UInventoryComponent* Inventory);
    bool RemoveItemFromInventory(UInventoryComponent* Inventory);
    bool DropItem(UInventoryComponent* Inventory);

    void SpawnDropItem(UInventoryComponent* Inventory);

    int32 ThisItemID = 99;
};

template<typename OWT, typename DT, typename TRT, typename WT>
inline bool TTemplateItem<OWT, DT, TRT, WT>::AddItemToInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID == 99)
    {
        if (!ItemTableRowInfo && ItemDynamicInfo.ItemTypeName != "None")
        {
            ItemTableRowInfo = Inventory->FindDataTableByStructType(OWT::StaticClass())->FindRow<TRT>(ItemDynamicInfo.ItemTypeName, "");
        }
        if (ItemTableRowInfo && Inventory->Inventory.Contains(ItemTableRowInfo->ItemContainerType))
        {
            if (ItemTableRowInfo->ItemContainerType == EContainerType::ClothingOne ||
                ItemTableRowInfo->ItemContainerType == EContainerType::ClothingTwo)
            {
                Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<InventoryItemClassBase*>(Owner);
                ThisItemID = 0;
                return true;
            }
            else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
            {
                TArray<InventoryItemClassBase*>& ContainerArray = Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.TryGet<FContainerBase>()->Inventory;
                if (!ContainerArray.IsEmpty())
                {
                    for (int32 Counter = 0; Counter < ContainerArray.Num(); Counter++)
                    {
                        if (!ContainerArray[Counter])
                        {
                            ContainerArray[Counter] = Owner;
                            ThisItemID = Counter;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

template<typename OWT, typename DT, typename TRT, typename WT>
inline bool TTemplateItem<OWT, DT, TRT, WT>::RemoveItemFromInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID != 99)
    {
        if (!ItemTableRowInfo && ItemDynamicInfo.ItemTypeName != "None")
        {
            ItemTableRowInfo = Inventory->FindDataTableByStructType(OWT::StaticClass())->FindRow<TRT>(ItemDynamicInfo.ItemTypeName, "");
        }
        if (Inventory->Inventory.Contains(ItemTableRowInfo->ItemContainerType))
        {
            if (ItemTableRowInfo->ItemContainerType == EContainerType::ClothingOne ||
                ItemTableRowInfo->ItemContainerType == EContainerType::ClothingTwo)
            {
                Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<InventoryItemClassBase*>(nullptr);
                ThisItemID = 99;
                return true;
            }
            else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
            {
                TArray<InventoryItemClassBase*> ContainerArray = Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.TryGet<FContainerBase>()->Inventory;

                if (!ContainerArray.IsEmpty())
                {
                    ContainerArray[ThisItemID] = nullptr;
                    ThisItemID = 99;
                    return true;
                }
            }
        }
    }
    return false;
}

template<typename OWT, typename DT, typename TRT, typename WT>
inline bool TTemplateItem<OWT, DT, TRT, WT>::DropItem(UInventoryComponent* Inventory)
{
    /**
    if (!ItemTableRowInfo && ItemDynamicInfo.ItemTypeName != "None")
    {
        ItemTableRowInfo = Inventory->FindDataTableByStructType(OWT::StaticClass())->FindRow<TRT>(ItemDynamicInfo.ItemTypeName, "");
    }
    if (ItemTableRowInfo && ItemTableRowInfo->bIsPlayerCanDropAndTakeIt)
    {
        if (RemoveItemFromInventory(Inventory))
        {
            SpawnDropItem(Inventory);

            return true;
        }
    }
    */
    return false;
}


template<typename OWT, typename DT, typename TRT, typename WT>
inline void TTemplateItem<OWT, DT, TRT, WT>::SpawnDropItem(UInventoryComponent* Inventory)
{
    WT* WorldItem = Inventory->GetWorld()->SpawnActor<WT>();
    WorldItem->SetActorTransform(Inventory->GetOwner()->GetActorTransform() + Inventory->PlayerDropLocationOffset);
    WorldItem->ItemDynamicInfo = ItemDynamicInfo;
}
