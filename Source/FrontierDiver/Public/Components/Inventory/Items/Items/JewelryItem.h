// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/Base/ItemBase.h"
#include "Components/Inventory/Items/Base/TemplateItem.h"
#include "JewelryItem.generated.h"

class AJewelryWorldItem;

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

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UJewelryItem : public UItemBase, public TTemplateItem<UJewelryItem, FJewelryItemDynamicInfo, FJewelryItemTableRowInfo, AJewelryWorldItem>
{
    GENERATED_BODY()

protected:

    UJewelryItem();

    virtual bool AddThisItemToInventory(UInventoryComponent* Inventory) override;

    virtual bool RemoveThisItemFromInventory(UInventoryComponent* Inventory) override;

    virtual bool DropThisItemFromInventory(UInventoryComponent* Inventory) override;

};
