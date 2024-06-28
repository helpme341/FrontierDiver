// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "InventoryComponent.generated.h"


UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
	None,
	BackMount,// ????????? ?? ?????
	SideMount,// ??????? ?????????
	ChestMount,// ????????? ?? ?????
	HipMount,// ????????? ?? ?????
	ThighMount,// ????????? ?? ?????
};

UENUM(BlueprintType)
enum class ECylinderInventorySlotType : uint8
{
	None,
	BackMount,
	SideMount,
	ChestMount,
	HipMount,
	ThighMount,
};


class ItemBase;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()

public:
	
	ItemBase* WearCostume;

	ItemBase* WearFlipper;

	TMap<EInventorySlotType, ItemBase*> Inventory;

	TMap<ECylinderInventorySlotType, ItemBase*> CylinderInventory;

	bool AddItemToInventory(ItemBase* Item);

	bool RemoveItemFromInventory(ItemBase* Item);

	bool DropItemFromInventory(ItemBase* Item);


};