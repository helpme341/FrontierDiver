// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/WorldItem.h"
#include <functional> 
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

USTRUCT(BlueprintType)
struct FSharedContainerBase
{
	GENERATED_BODY()

	TStrongObjectPtr<UItemBase> Item;
};


USTRUCT(BlueprintType)
struct FContainerBase
{
	GENERATED_BODY()

	TArray<FSharedContainerBase> ContainerInventory;

	FContainerBase() {}

    FContainerBase(int32 InventorySlots)
	{
		for (int32 Counter = 0; Counter < InventorySlots; Counter++)
		{
			ContainerInventory.Add(FSharedContainerBase());
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
	float PlayerDropLocationOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	EContainerType QuickInventoryContainerType;
	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI") 
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() { return InventoryWidget.Get(); }

	UFUNCTION(BlueprintCallable)
    AFrontierDiverCharacter* GetOwnerCharacter() { return Cast<AFrontierDiverCharacter>(GetOwner()); }

	UFUNCTION(BlueprintCallable)
	bool TakeItemToHandsByID(int32 ID);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromHands();

	void BeginPlay() override;
	int AddItemToInventory(TStrongObjectPtr<UItemBase> Item, TStrongObjectPtr<UItemBase>& ItemResult);
	int RemoveItemFromInventory(TStrongObjectPtr<UItemBase> Item);
	bool PickupItemToInventory(TStrongObjectPtr<AWorldItem> Item);
	int DropItemFromInventory(TStrongObjectPtr<UItemBase> Item);

	bool FirstInteractWithHeldItem();
	bool SecondInteractWithHeldItem();
	bool ThirdInteractWithHeldItem();

	TMap<EContainerType, FContainerBase> Inventory{
	  { EContainerType::ClothingOne, FContainerBase(1)},
	  { EContainerType::ClothingTwo, FContainerBase(1)},
	  { EContainerType::Array, FContainerBase(5)}
	};
	TUniquePtr<UInventoryWidget> InventoryWidget;
	TStrongObjectPtr<UItemBase> HeldItem;
	bool bIsItemHeld;
};

