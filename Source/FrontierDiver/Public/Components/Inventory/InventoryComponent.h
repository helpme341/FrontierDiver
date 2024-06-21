// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/ItemsObjectTypes/InventoryItemObjectBase.h"
#include "InventoryComponent.generated.h"

class UMainInventoryItemObjectBase;
enum class EInventorySlotType : uint8;
enum class ECylinderInventorySlotType: uint8;
class UInventoryItemCylinderObject;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="DataTables")
	UDataTable* InventoryOutfitSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTables")
	UDataTable* InventoryCylinderSettings;

	TMap<EInventorySlotType, UMainInventoryItemObjectBase*> Inventory;

	TMap<ECylinderInventorySlotType, UInventoryItemCylinderObject*> CylinderInventory;
	
	bool AddItemToInvenory();

	//void RemoveItemFromInventory(FItemVariant ItemVariant);
};
