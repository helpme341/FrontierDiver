// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponent.h"

/**
 *  OWT = Owner Type (class)
 *  DT = Owner Dynamic Info Type (struct)
 *  DTT = Owner Table Row Type (struct)
 *  WT = Owner World Actor Type (class)
 */
template<typename OWT, typename DT, typename TRT, typename WT = AActor>
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

class UItemBase;



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
                Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<UItemBase*>(Owner);
                ThisItemID = 0;
                return true;
            }
            else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
            {
                if (!Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Get<FContainerBase>().Inventory.IsEmpty())
                {
                    for (int32 Counter = 0; Counter < Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Get<FContainerBase>().Inventoryr.Num(); Counter++)
                    {
                        if (!Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Get<FContainerBase>().Inventory[Counter])
                        {
                            Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<FContainerBase>();/////////////////////////
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
                Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Set<UItemBase*>(nullptr);
                ThisItemID = 99;
                return true;
            }
            else if (ItemTableRowInfo->ItemContainerType == EContainerType::Array)
            {
                TArray<UItemBase*> Container = Inventory->Inventory[ItemTableRowInfo->ItemContainerType].Item.Get<FContainerBase>().Inventory;

                if (!Container.IsEmpty())
                {
                    Container[ThisItemID] = nullptr;
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
