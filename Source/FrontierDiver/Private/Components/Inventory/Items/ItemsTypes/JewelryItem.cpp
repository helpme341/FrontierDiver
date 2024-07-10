// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Components/Inventory/InventoryComponent.h"

UJewelryItem::UJewelryItem()
{
	TItemTmpl::Owner = this;
	TPickupDropItemIFTmplImpl::Owner = this;
}

bool UJewelryItem::PickupItem(UInventoryComponent* Inventory, AWorldItem* Item)
{
	return TPickupDropItemIFTmplImpl::PickupItem(Inventory, Item);
}

bool UJewelryItem::DropItem(UInventoryComponent* Inventory)
{
	return TPickupDropItemIFTmplImpl::DropItem(Inventory);
}

bool UJewelryItem::FindDataTableByItemType()
{
	return BaseFindDataTableByItemType<UJewelryItem, FJewelryItemTableRowInfo>();
}

FItemTableRowInfoBase* UJewelryItem::GetItemStaticInfo()
{
	return ItemTableRowInfo;
}

FItemDynamicInfoBase* UJewelryItem::GetItemDynamicInfo()
{
	return &ItemDynamicInfo;
}
