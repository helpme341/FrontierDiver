// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"

bool UItemBase::FindDataTableByItemType()
{
    return false;
}

void  UItemBase::UpdateItemWidget(UInventoryItemWidget* ItemWidget)
{
}

bool UItemBase::AddThisItemToInventory(UInventoryComponent* Inventory)
{
    return false;
}

bool UItemBase::RemoveThisItemFromInventory(UInventoryComponent* Inventory, bool DestroyItem)
{
    return false;
}
