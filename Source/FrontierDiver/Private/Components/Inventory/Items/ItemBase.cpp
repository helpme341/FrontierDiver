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
    return false;
}

bool UItemBase::RemoveThisItemFromInventory(UInventoryComponent* Inventory, bool DestroyItem)
{
    return false;
}
