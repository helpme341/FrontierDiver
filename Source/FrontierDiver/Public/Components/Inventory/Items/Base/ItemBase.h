// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EContainerType : uint8
{
    Slot,
    ClothingOne,
    ClothingTwo,
    Array,
};

USTRUCT(BlueprintType)
struct FItemDynamicInfoBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName ItemTypeName;

};

USTRUCT(BlueprintType)
struct FItemStaticInfoBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool bIsPlayerCanDropAndTakeIt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    EContainerType ItemContainerType;
};

USTRUCT(BlueprintType)
struct FItemTableRowInfoBase : public FTableRowBase
{
    GENERATED_BODY()


};


class AWorldItemBase;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UItemBase : public UObject
{
	GENERATED_BODY()

public: 

	virtual bool AddThisItemToInventory(UInventoryComponent* Inventory);

	virtual bool RemoveThisItemFromInventory(UInventoryComponent* Inventory);

	virtual bool DropThisItemFromInventory(UInventoryComponent* Inventory);
	
};
