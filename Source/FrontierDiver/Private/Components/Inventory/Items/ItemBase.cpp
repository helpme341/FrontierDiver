// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Items/WorldItem.h"

bool UItemBase::FindDataTableByItemType()
{
    return false;
}

const FTransform& UItemBase::GetWorldItemOffset()
{
    static const FTransform DefaultTransform = FTransform();
    return DefaultTransform;
}
