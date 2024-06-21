// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/ItemsObjectTypes/InventoryItemObjectBase.h"
#include "InventoryCylinderItemObjectBase.generated.h"

UENUM(BlueprintType)
enum class ECylinderSlotType : uint8
{
	None,
	BackMount,// ????????? ?? ?????
	SideMount,// ??????? ?????????
	ChestMount,// ????????? ?? ?????
	HipMount,// ????????? ?? ?????
	ThighMount,// ????????? ?? ?????
};

UENUM(BlueprintType)
enum class EWeightOfCylinder : uint8
{
	None,
	VeryLight,
	Light,
	Medium,
	Heavy,
	VeryHeavy,
};

USTRUCT(BlueprintType)
struct  FInventoryDinamicCylinderItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	float NowCapacity;
};

USTRUCT(BlueprintType)
struct FInventorynStaticCylinderItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	float MaxCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	EWeightOfCylinder WeightOfCylinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	ECylinderSlotType CylinderSlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	float ValuableOfCylinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerOfUse")
	float PowerOfUseAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerOfUse")
	float PowerOfUseNitrox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerOfUse")
	float PowerOfUseTrimix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerOfUse")
	float PowerOfUseNelix;
};

USTRUCT(BlueprintType)
struct  FInventoryCylinderDataTibleSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventorynStaticCylinderItem InventorynStaticCylinderItem;
};

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryCylinderItemObjectBase : public UInventoryItemObjectBase
{
	GENERATED_BODY()
	
};
