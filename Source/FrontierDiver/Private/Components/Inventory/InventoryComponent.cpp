// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/Bases/ItemBase.h"
#include "Components/Inventory/Items/Interfaces/PickupDropItemIFBase.h"

bool UInventoryComponent::AddItemToInventory(ItemBase* Item)
{
	if (Item->AddThisItemToInventory(this)) { return true; }
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(ItemBase* Item)
{
	if (Item->RemoveThisItemFromInvenory(this)) { return true; }
	return false;
}

bool UInventoryComponent::DropItemFromInventory(ItemBase* Item)
{
	IPickupDropItemIFBase* TEst =  dynamic_cast<IPickupDropItemIFBase*>(Item);
}
