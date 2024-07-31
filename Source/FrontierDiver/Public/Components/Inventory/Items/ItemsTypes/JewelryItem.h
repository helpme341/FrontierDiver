// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "JewelryItem.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FJewelryItemTableRowInfo : public FItemTableRowInfoBase
{
	GENERATED_BODY()
};


/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UJewelryItem : 
    public UItemBase, 
    public TItemTmpl<UItemDynamicInfo, FJewelryItemTableRowInfo>
{
    GENERATED_BODY()

public:

    UJewelryItem();

    bool FindDataTableForItem(UWorld* World)
        override { return BaseFindDataTableForItem<UJewelryItem, FJewelryItemTableRowInfo>(World); }

    const FItemTableRowInfoBase* GetItemStaticInfo() override;

    UItemDynamicInfo* GetItemDynamicInfo() override;

    void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) override;
};
 