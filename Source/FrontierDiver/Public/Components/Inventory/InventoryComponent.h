// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/Items/Base/ItemBase.h"
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
struct FContainerBase
{
	GENERATED_BODY()

	TArray<UItemBase*> Inventory;
};

UCLASS()
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<UItemBase>, UDataTable* > DataTablesInfo;

	TMap<EContainerType, TVariant<UItemBase*, FContainerBase>> Inventory;


	FTransform PlayerDropLocationOffset;

	bool AddItemToInventory(UItemBase* Item);

	bool AddItemToInventory(AWorldItemBase* Item);

	bool RemoveItemFromInventory(UItemBase* Item);

	bool DropItemFromInventory(UItemBase* Item);

	UDataTable* FindDataTableByStructType(TSubclassOf<UItemBase> Item);
};
