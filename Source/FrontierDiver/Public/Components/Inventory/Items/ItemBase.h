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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    FName ItemTypeName = "None";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 QuantityItems = 0;

};

USTRUCT(BlueprintType)
struct FItemTableRowInfoBase : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    UTexture2D* ItemWidgetTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    UStaticMesh* ItemWorldStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    EContainerType ItemContainerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 MaxQuantityItemsInSlot = 1;
};

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

    int32 ThisItemID = 99;

    virtual bool FindDataTableByItemType(UInventoryComponent* Inventory);

    virtual FItemTableRowInfoBase* GetItemStaticInfo();

    virtual FItemDynamicInfoBase* GetItemDynamicInfo();

    bool bUseCustomAddThisItemToInventory = false;

    bool bUseCustomRemoveThisItemFromInventory = false;

    bool bUseCustomUpdateWidgetForThisItem = false;

    virtual void UpdateItemWidget(class UInventoryItemWidget* ItemWidget);

	virtual bool AddThisItemToInventory(UInventoryComponent* Inventory);

	virtual bool RemoveThisItemFromInventory(UInventoryComponent* Inventory, bool DestroyItem);

};
