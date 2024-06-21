// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/ItemsObjectTypes/MainInventory/MainInventoryItemObjectBase.h"
#include "MainInventoryOutfitItemObject.generated.h"

USTRUCT(BlueprintType)
struct FInventoryDinamicOutfitItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemRowName;
};

USTRUCT(BlueprintType)
struct FInventoryStaticOutfitItem
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TMap<ECylinderSlotType, FItemVariant> InventoryOxygenCylinderSlotTypes;
};

USTRUCT(BlueprintType)
struct  FInventoryOutfitItemDataTibleSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryStaticOutfitItem InventoryStaticOutfitItem;
};

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UMainInventoryOutfitItemObject : public UMainInventoryItemObjectBase
{
	GENERATED_BODY()
	
};
