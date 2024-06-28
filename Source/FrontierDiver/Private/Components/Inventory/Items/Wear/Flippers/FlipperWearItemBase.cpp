// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/Wear/Flippers/FlipperWearItemBase.h"
#include <Components/Inventory/InventoryComponent.h>

FlipperWearItemBase::FlipperWearItemBase()
{
}

FlipperWearItemBase::~FlipperWearItemBase()
{
}

bool FlipperWearItemBase::AddThisItemToInventory(UInventoryComponent* Inventory)
{
	/**
	if (!Inventory->WearFlipper)
	{
		Inventory->WearFlipper = this;
		return true;
	}
	return false;
	*/
}

bool FlipperWearItemBase::RemoveThisItemFromInvenory(UInventoryComponent* Inventory)
{
	/**
	if (Inventory->WearFlipper == this)
	{
		Inventory->WearFlipper = nullptr;
		return true;
	}
	return false;
	*/
}
