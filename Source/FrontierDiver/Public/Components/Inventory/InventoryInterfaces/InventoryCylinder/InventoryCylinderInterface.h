// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/Inventory/InventoryInterfaces/InventoryInterfaceBase.h"
#include "InventoryCylinderInterface.generated.h"

UENUM(BlueprintType)
enum class EInventorynCylinderSlotType : uint8
{
	None,
	BackMount,// Крепление на спину
	SideMount,// Боковое крепление
	ChestMount,// Крепление на грудь
	HipMount,// Крепление на бедро
	ThighMount,// Крепление на бедро
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

// Basic Architecture for Distributing Item Data into Static Data Tables and Denam Data 

// oxygen cylinder

USTRUCT(BlueprintType)
struct FInventorynCylinderDataTibleBase : public FItemDataTibleBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	float MaxCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	EWeightOfCylinder WeightOfCylinder;

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
struct  FCylinderDataTibleSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventorynCylinderDataTibleBase InventorynCylinderDataTibleBase;
};

USTRUCT(BlueprintType)
struct  FInventoryCylinderInfo : public FItemInfoBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "None")
	float NowCapacity;

	FInventoryCylinderInfo()
	{
		InventoryItemClass = EInventoryItemClass::Cylinder;
	}
};

UINTERFACE(MinimalAPI)
class UInventoryCylinderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IInventoryCylinderInterface : public IInventoryInterfaceBase
{
	GENERATED_BODY()
};
