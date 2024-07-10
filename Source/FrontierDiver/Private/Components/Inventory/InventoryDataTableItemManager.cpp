// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryDataTableItemManager.h"
#include "Components/Inventory/Items/ItemBase.h"

AInventoryDataTableItemManager::AInventoryDataTableItemManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

UDataTable* AInventoryDataTableItemManager::FindDataTableByItemType(TSubclassOf<UItemBase> Item)
{
	if (DataTablesInfo.Contains(Item)) { return DataTablesInfo[Item]; }
	return nullptr;
}
