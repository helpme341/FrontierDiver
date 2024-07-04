// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Components/Inventory/InventoryComponent.h"

UJewelryItem::UJewelryItem()
{
	TItemTmpl::Owner = this;
	TPickupDropItemIFTmplImpl::Owner = this;
}

bool UJewelryItem::FindDataTableByItemType(UInventoryComponent* Inventory)
{
	if (!ItemTableRowInfo && ItemDynamicInfo.ItemTypeName != "None")
	{
		ItemTableRowInfo = Inventory->FindDataTableByItemType(UJewelryItem::StaticClass())->FindRow<FJewelryItemTableRowInfo>(ItemDynamicInfo.ItemTypeName, "");
		return true;
	}
	return false;
}

FItemTableRowInfoBase* UJewelryItem::GetItemStaticInfo()
{
	return ItemTableRowInfo;
}
