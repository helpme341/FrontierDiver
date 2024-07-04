// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/Items/ItemBase.h"
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

	TArray<UItemBase*> Inventory;

	FContainerBase() {}

	FContainerBase(const TArray<UItemBase*>& InInventory)
	{
		Inventory = InInventory;
	}

	FContainerBase(int32 InventorySlots)
	{
		for (int32 Counter = 0; Counter < InventorySlots; Counter++)
		{
			Inventory.Add(nullptr);
		}
	}
};

struct FItemVariantBase
{
	TVariant<UItemBase*, FContainerBase> Item;
	

	FItemVariantBase() {}

	FItemVariantBase(UItemBase* InItem)
	{
		Item.Set<UItemBase*>(InItem);
	}

	FItemVariantBase(const FContainerBase& InContainerBase)
	{
		Item.Set<FContainerBase>(InContainerBase);
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

	TMap<EContainerType, FItemVariantBase> Inventory{
		  { EContainerType::ClothingOne, FItemVariantBase()},
		  { EContainerType::ClothingTwo, FItemVariantBase(nullptr)},
		  { EContainerType::Array, FItemVariantBase(FContainerBase(5))}
	};

	FTransform PlayerDropLocationOffset;

	bool AddItemToInventory(UItemBase* Item);

	bool RemoveItemFromInventory(UItemBase* Item, bool DestroyItem);

	bool PickupItemToInventory(AWorldItem* Item);

	bool DropItemFromInventory(UItemBase* Item);

	UDataTable* FindDataTableByItemType(TSubclassOf<UItemBase> Item);
};
