// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponent.h"

/**
 *  OWT = Owner Type (class)
 *  DT = Owner Dynamic Info Type (struct)
 *  ST = Owner Static Info Type (struct)
 *  DTT = Owner Table Row Type (struct)
 *  WT = Owner World Actor Type (class)
 */
template<typename OWT, typename DT, typename ST, typename DTT, typename WT = AActor>
class FRONTIERDIVER_API TTemplateItem
{
public:
    DT ItemDynamicInfo;
    ST* ItemStaticInfo;
    OWT* Owner;

    bool AddItemToInventory(UInventoryComponent* Inventory);
    bool RemoveItemFromInventory(UInventoryComponent* Inventory);
    bool DropItem(UInventoryComponent* Inventory);

    void SpawnDropItem(UInventoryComponent* Inventory);

    int32 ThisItemID = 99;
};

class UItemBase;


/**
* OWT = Owner Class
* 
*/
template<typename OWT, typename DT, typename ST, typename DTT, typename WT>
inline bool TTemplateItem<OWT, DT, ST, DTT, WT>::AddItemToInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID == 99)
    {
        if (!ItemStaticInfo && ItemDynamicInfo.ItemTypeName != "None")
        {
            ItemStaticInfo = Inventory->FindDataTableByStructType(OWT::StaticClass())->FindRow<ST>(ItemDynamicInfo.ItemTypeName, "");
        }
        if (ItemStaticInfo)
        {
            if (ItemStaticInfo->ItemContainerType == EContainerType::ClothingOne ||
                ItemStaticInfo->ItemContainerType == EContainerType::ClothingTwo)
            {
                Inventory->Inventory[ItemStaticInfo->ItemContainerType].Set<UItemBase*>(Owner);
            }
            else if (ItemStaticInfo->ItemContainerType == EContainerType::Array)
            {
                TArray<UItemBase*>& Container = Inventory->Inventory[ItemStaticInfo->ItemContainerType].Get<FContainerBase>().Inventory;;
                if (!Container.IsEmpty())
                {
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
            }
        }
    }
    return false;
}

template<typename OWT, typename DT, typename ST, typename DTT, typename WT>
inline bool TTemplateItem<OWT, DT, ST, DTT, WT>::RemoveItemFromInventory(UInventoryComponent* Inventory)
{
    if (ThisItemID != 99)
    {
        /**
        Inventory->Inventory.Find(ItemStaticInfo->ItemContainerType)->Inventory[ThisItemID] = nullptr;
        Inventory->Inventory.Find(ItemStaticInfo->ItemContainerType)->Inventory[ThisItemID]->ConditionalBeginDestroy();
        ThisItemID = 99;
        return true;
        */
    }
    return false;
}

template<typename OWT, typename DT, typename ST, typename DTT, typename WT>
inline bool TTemplateItem<OWT, DT, ST, DTT, WT>::DropItem(UInventoryComponent* Inventory)
{
    if (!ItemStaticInfo && ItemDynamicInfo.ItemTypeName != "None")
    {
        ItemStaticInfo = Inventory->FindDataTableByStructType(OWT::StaticClass())->FindRow<ST>(ItemDynamicInfo.ItemTypeName, "");
    }
    if (ItemStaticInfo && ItemStaticInfo->bIsPlayerCanDropAndTakeIt)
    {
        if (RemoveItemFromInventory(Inventory))
        {
            SpawnDropItem(Inventory);

            return true;
        }
    }
    return false;
}


template<typename OWT, typename DT, typename ST, typename DTT, typename WT>
inline void TTemplateItem<OWT, DT, ST, DTT, WT>::SpawnDropItem(UInventoryComponent* Inventory)
{
    WT* WorldItem = Inventory->GetWorld()->SpawnActor<WT>();
    WorldItem->SetActorTransform(Inventory->GetOwner()->GetActorTransform() + Inventory->PlayerDropLocationOffset);
    WorldItem->ItemDynamicInfo = ItemDynamicInfo;
}
