// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Components/Inventory/InventoryComponent.h"

UJewelryItem::UJewelryItem()
{
	bIsPlayerCanDropThisItem = true;
}

const FItemTableRowInfoBase* UJewelryItem::GetItemStaticInfo()
{
	return ItemTableRowInfo;
}

FItemDynamicInfoBase& UJewelryItem::GetItemDynamicInfo()
{
	return ItemDynamicInfo;
}
