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

UItemDynamicInfo* UJewelryItem::GetItemDynamicInfo()
{
	return ItemDynamicInfo;
}

void UJewelryItem::SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo)
{
	ItemDynamicInfo = DynamicInfo;
}
