// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/WorldItem.h"
#include <functional> 
#include "InventoryComponent.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	UInventoryComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float PlayerDropLocationOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	EContainerType QuickInventoryContainerType = EContainerType::QuickInventory;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI") 
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() { return InventoryWidget.Get(); }

	UFUNCTION(BlueprintCallable)
    AFrontierDiverCharacter* GetOwnerCharacter() { return Cast<AFrontierDiverCharacter>(GetOwner()); }

	UFUNCTION(BlueprintCallable)
	bool HeldItemToHandsByID(int32 ID);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromHands();


	UFUNCTION(BlueprintCallable)
	bool HeldBreathingTankItemByID(int32 ID);


	UFUNCTION(BlueprintCallable)
	bool RemoveBreathingTankItem();


	void BeginPlay() override;
	int AddItemToInventory(UItemBase* Item, UItemBase*& ItemResult);
	int RemoveItemFromInventory(UItemBase* Item);
	bool PickupItemToInventory(AWorldItem* Item);
	int DropItemFromInventory(UItemBase* Item);

	bool FirstInteractWithHeldItem();
	bool SecondInteractWithHeldItem();
	bool ThirdInteractWithHeldItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EContainerType, FContainerBase> Inventory{
	  { EContainerType::Clothing, FContainerBase(1)},
	  { EContainerType::Flippers, FContainerBase(1)},
	  { EContainerType::BreathingTanks, FContainerBase(1)},
	  { EContainerType::QuickInventory, FContainerBase(3)}
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCounterOfCreatedWidgets = 0;

	void OnInventoryItemWidgetConstructed();
	TStrongObjectPtr<UInventoryWidget> InventoryWidget;
	TStrongObjectPtr<UItemBase> HeldItem;
	class TStrongObjectPtr<UBreathingTankItem> HeldBreathingTankItem;
	
	UPROPERTY()
	bool bIsItemHeld;

	UPROPERTY()
	bool bIsBreathingTankItemHeld;

private:

	int32 CounterOfCreatedWidgets = 0;
};

