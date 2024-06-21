// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "InventoryComponent.generated.h"

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




UENUM(BlueprintType)
enum class ECylinderSlotType : uint8
{
	None,
	BackMount,// ��������� �� �����
	SideMount,// ������� ���������
	ChestMount,// ��������� �� �����
	HipMount,// ��������� �� �����
	ThighMount,// ��������� �� �����
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


USTRUCT(BlueprintType)
struct FItemVariant
{
	GENERATED_BODY()

	TVariant<FInventoryDinamicOutfitItem, FInventoryDinamicCylinderItem> Item;


	bool bIsThisItemEmpty = true;
};

UENUM(BlueprintType)
enum class EMainInventorySlotType : uint8
{
	Outfit,
	b,
	c,
};

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="DataTables")
	UDataTable* InventoryOutfitSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTables")
	UDataTable* InventoryCylinderSettings;

	TMap<EMainInventorySlotType, FItemVariant> MainInventory;

	TMap<ECylinderSlotType, FItemVariant> CylinderInventory;
	
	bool AddItemToInvenory(FItemVariant ItemVariant);

	//void RemoveItemFromInventory(FItemVariant ItemVariant);
};
