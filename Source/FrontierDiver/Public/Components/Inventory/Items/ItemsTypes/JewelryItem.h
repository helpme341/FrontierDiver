// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIFTmplImpl.h"
#include "JewelryItem.generated.h"

USTRUCT(BlueprintType)
struct FJewelryItemDynamicInfo : public FItemDynamicInfoBase
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FJewelryItemTableRowInfo : public FItemTableRowInfoBase
{
    GENERATED_BODY()
};

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UJewelryItem : public UItemBase,
    public TItemTmpl<UJewelryItem, FJewelryItemDynamicInfo, FJewelryItemTableRowInfo>,
    public TPickupDropItemIFTmplImpl<UJewelryItem, FJewelryItemDynamicInfo, FJewelryItemTableRowInfo>
{
    GENERATED_BODY()

public:

    UJewelryItem();

    bool FindDataTableByItemType(UInventoryComponent* Inventory) override;

    FItemTableRowInfoBase* GetItemStaticInfo() override;

};
