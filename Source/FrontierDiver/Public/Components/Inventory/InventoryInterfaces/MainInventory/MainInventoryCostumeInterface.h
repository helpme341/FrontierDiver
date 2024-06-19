// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/Inventory/InventoryInterfaces/MainInventory/MainInventoryInterfaceBase.h"
#include "MainInventoryCostumeInterface.generated.h"

UENUM(BlueprintType)
enum class ECostumeItemType : uint8
{
	None,
	test1,
	test2,
};

USTRUCT(BlueprintType)
struct  FCostumeItemDataTibleBase : public FMainInventoryItemDataTibleBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECostumeItemType CostumeType;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TMap<EInventorynCylinderSlotType, FItemInfoBase> InventoryOxygenCylinderSlotTypes;

	FCostumeItemDataTibleBase()
	{
		MainInventorySlotTypes = { EMainInventorySlotType::Costume };
	}
};

USTRUCT(BlueprintType)
struct  FCostumeItemDataTibleSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCostumeItemDataTibleBase CostumeItemSettings;
};


UINTERFACE(MinimalAPI)
class UMainInventoryCostumeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IMainInventoryCostumeInterface
{
	GENERATED_BODY()
public:
};
