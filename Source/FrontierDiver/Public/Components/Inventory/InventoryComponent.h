// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/WorldItem.h"
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

struct FContainerBase
{

	TArray<UItemBase*> ContainerInventory;

	FContainerBase() {}


    FContainerBase(int32 InventorySlots)
	{
		for (int32 Counter = 0; Counter < InventorySlots; Counter++)
		{
			ContainerInventory.Add(nullptr);
		}
	}
};

class UItemBase;
class AWorldItem;

UCLASS()
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<UItemBase>, UDataTable* > DataTablesInfo;

	TMap<EContainerType, FContainerBase> Inventory{
		  { EContainerType::ClothingOne, FContainerBase(1)},
		  { EContainerType::ClothingTwo, FContainerBase(1)},
		  { EContainerType::Array, FContainerBase(5)}
	};

	FTransform PlayerDropLocationOffset;

	bool AddItemToInventory(UItemBase* Item);

	bool RemoveItemFromInventory(UItemBase* Item, bool DestroyItem);

	bool PickupItemToInventory(AWorldItem* Item);

	bool DropItemFromInventory(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	UDataTable* FindDataTableByItemType(TSubclassOf<UItemBase> Item);

private:
	// бызывие релизации функций придметов 

	bool BaseAddItemToInventory(UItemBase* NewItem);

	bool BaseRemoveItemFromInventory(UItemBase* NewItem, bool DestroyItem);
};

