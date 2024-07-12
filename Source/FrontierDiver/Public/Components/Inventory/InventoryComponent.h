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

class AWorldItem;
class UInventoryWidget;
class AFrontierDiverCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All);

UCLASS()
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FVector PlayerDropLocationOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	EContainerType QuickInventoryContainerType;
	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI") 
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() { return InventoryWidget; }
	UFUNCTION(BlueprintCallable)
	AFrontierDiverCharacter* GetOwnerCharacter() { return Cast<AFrontierDiverCharacter>(GetOwner()); }
	UFUNCTION(BlueprintCallable)
	bool TakeItemToHandsByID(int32 ID);
	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromHands();


	void BeginPlay() override;
	bool AddItemToInventory(UItemBase* Item, bool DestroyItem);
	bool RemoveItemFromInventory(UItemBase* Item, bool DestroyItem);
	bool PickupItemToInventory(AWorldItem* Item);
	bool DropItemFromInventory(UItemBase* Item);

	TMap<EContainerType, FContainerBase> Inventory{
	  { EContainerType::ClothingOne, FContainerBase(1)},
	  { EContainerType::ClothingTwo, FContainerBase(1)},
	  { EContainerType::Array, FContainerBase(5)}
	};
	UInventoryWidget* InventoryWidget;
	UItemBase* HeldItem;
	bool bIsItemHeld;

private:

	bool BaseAddItemToInventory(UItemBase* NewItem, bool DestroyItem);
	bool BaseRemoveItemFromInventory(UItemBase* NewItem, bool DestroyItem);
};

