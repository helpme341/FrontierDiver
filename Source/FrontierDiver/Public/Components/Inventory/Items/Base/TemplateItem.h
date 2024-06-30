// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponent.h"

/**
 * 
 */
template<typename DT, typename ST, typename DTT, typename WT = AActor>
class FRONTIERDIVER_API TTemplateItem
{
public:
    DT ItemDynamicInfo;
    ST* ItemStaticInfo;
    UItemBase* Owner;

    bool AddThisItemToInventory(UInventoryComponent* Inventory);
    bool RemoveThisItemFromInventory(UInventoryComponent* Inventory);
    bool DropThisItem(UInventoryComponent* Inventory);

    void SpawnDropItem(UInventoryComponent* Inventory);

    int32 ThisItemID = 99;
};

class UItemBase;

template<typename DT, typename ST, typename DTT, typename WT>
inline bool TTemplateItem<DT, ST, DTT, WT>::AddThisItemToInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID == 99)
    {
        TArray<UItemBase*>& Container = Inventory->Inventory.Find(ItemStaticInfo->ItemContainerType)->Inventory;
        for (int32 Counter = 0; Counter < Container.Num(); Counter++)
        {
            if (!Container[Counter])
            {
                Container[Counter] = Owner;
                ThisItemID = Counter;
                return true;
            }
        }
    }
    return false;
}

template<typename DT, typename ST, typename DTT, typename WT>
inline bool TTemplateItem<DT, ST, DTT, WT>::RemoveThisItemFromInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID != 99)
    {
        Inventory->Inventory.Find(ItemStaticInfo->ItemContainerType)->Inventory[ThisItemID] = nullptr;
        Inventory->Inventory.Find(ItemStaticInfo->ItemContainerType)->Inventory[ThisItemID]->ConditionalBeginDestroy();
        ThisItemID = 99;
        return true;
    }
    return false;
}

template<typename DT, typename ST, typename DTT, typename WT>
inline bool TTemplateItem<DT, ST, DTT, WT>::DropThisItem(UInventoryComponent* Inventory)
{
    if (!ItemStaticInfo && ItemDynamicInfo.ItemTypeName != "None")
    {
        ItemStaticInfo = Inventory->FindDataTableByStructType<DTT>()->FindRow<ST>(ItemDynamicInfo.ItemTypeName, "");
    }
    if (ItemStaticInfo && ItemStaticInfo->bIsPlayerCanDropAndTakeIt)
    {
        if (RemoveThisItemFromInventory(Inventory))
        {
            SpawnDropItem(Inventory);

            return true;
        }
    }
    return false;
}


template<typename DT, typename ST, typename DTT, typename WT>
inline void TTemplateItem<DT, ST, DTT, WT>::SpawnDropItem(UInventoryComponent* Inventory)
{
    WT* WorldItem = Inventory->GetWorld()->SpawnActor<WT>();
    WorldItem->SetActorTransform(Inventory->GetOwner()->GetActorTransform() + Inventory->PlayerDropLocationOffset);
    WorldItem->ItemDynamicInfo = ItemDynamicInfo;
}
