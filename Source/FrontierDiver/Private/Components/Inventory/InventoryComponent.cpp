// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"

bool UInventoryComponent::AddItemToInvenory(FItemVariant ItemVariant)
{
    // ���������� ����� Get ��� ��������� FInventoryCylinderInfo
    if (ItemVariant.Item.IsType<FInventoryDinamicOutfitItem>())
    {
        if (CylinderInventory.IsEmpty())
        {
            const FInventoryDinamicOutfitItem& CostumeInfo = ItemVariant.Item.Get<FInventoryDinamicOutfitItem>();
            if (MainInventory[EMainInventorySlotType::Outfit].bIsThisItemEmpty)
            {
                MainInventory[EMainInventorySlotType::Outfit].Item.Set<FInventoryDinamicOutfitItem>(CostumeInfo);
                //CylinderInventory = InventoryOutfitSettings->FindRow<FInventoryOutfitItemDataTibleSettings>(CostumeInfo.ItemRowName, "", true)->InventoryStaticOutfitItem.InventoryOxygenCylinderSlotTypes;
                MainInventory[EMainInventorySlotType::Outfit].bIsThisItemEmpty = false;
                return true;
            }
        }
    }
    else if (ItemVariant.Item.IsType<FInventoryDinamicCylinderItem>())
    {
        if (!CylinderInventory.IsEmpty())
        {
            const FInventoryDinamicCylinderItem& CylinderInfo = ItemVariant.Item.Get<FInventoryDinamicCylinderItem>();
            ECylinderSlotType CylinderSlotType = InventoryCylinderSettings->FindRow<FInventoryCylinderDataTibleSettings>(CylinderInfo.ItemRowName, "", true)->InventorynStaticCylinderItem.CylinderSlotType;
            if (CylinderInventory[CylinderSlotType].bIsThisItemEmpty)
            {
                CylinderInventory[CylinderSlotType].Item.Set<FInventoryDinamicCylinderItem>(CylinderInfo);
                CylinderInventory[CylinderSlotType].bIsThisItemEmpty = false;
                return true;
            }
        }
    }
    return false;
}
