// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/TextBlock.h"
#include "BreathingTankItem.generated.h"

UCLASS()
class FRONTIERDIVER_API UBreathingTankItemDynamicInfo : public UItemDynamicInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	float CurrentAir = 100.0f;
};

USTRUCT(BlueprintType)
struct FBreathingTankItemTableRowInfo : public FItemTableRowInfoBase
{
	GENERATED_BODY()

	FBreathingTankItemTableRowInfo()
	{
		MaxQuantityItemsInSlot = 1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomBreathingTankSettings")
	float MaxAir = 100.0f;


};


/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UBreathingTankItem : public UItemBase,
	public TItemTmpl<UBreathingTankItemDynamicInfo, FBreathingTankItemTableRowInfo>
{
	GENERATED_BODY()
public:

	UBreathingTankItem();

	bool FindDataTableForItem(UWorld* World)
		override { return BaseFindDataTableForItem<UBreathingTankItem, FBreathingTankItemTableRowInfo>(World); }

	const FItemTableRowInfoBase* GetItemStaticInfo()
		override { return ItemTableRowInfo.Get(); }

	UItemDynamicInfo* GetItemDynamicInfo() 
		override { return ItemDynamicInfo.Get(); }
	
	void OnAddItemToInventory() override;

	void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) override;

	float GetBreathingTankMaxAir() { return ItemTableRowInfo->MaxAir; }


	void CustomUpdateWidget() override;

	void CustomShowItemInfo()  override;

	void CustomUpdateItemInfo()  override;


	void SetBreathingTankAir(float Amount)
	{
		ItemDynamicInfo->CurrentAir = FMath::Clamp(ItemDynamicInfo->CurrentAir + Amount, 0.0f, ItemTableRowInfo->MaxAir);
		ItemWidget->InventoryWidget->UpdateItemInfo();
		if(ItemWidget.IsValid()) { ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ItemDynamicInfo->CurrentAir))); }
	}

	float GetBreathingTankAir() { return ItemDynamicInfo->CurrentAir; }
};
