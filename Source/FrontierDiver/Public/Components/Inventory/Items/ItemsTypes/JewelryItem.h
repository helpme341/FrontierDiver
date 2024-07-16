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
    public TItemTmpl<FItemDynamicInfoBase, FJewelryItemTableRowInfo>
{
    GENERATED_BODY()

public:

    UJewelryItem();

    bool FindDataTableByItemType() 
        override { return BaseFindDataTableByItemType<UJewelryItem, FJewelryItemTableRowInfo>(); }

    const FItemTableRowInfoBase* GetItemStaticInfo() override;

    FItemDynamicInfoBase& GetItemDynamicInfo() override;
};
 