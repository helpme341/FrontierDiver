// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "BreathingTankItem.generated.h"

UCLASS()
class FRONTIERDIVER_API UBreathingTankItemDynamicInfo : public UItemDynamicInfo
{
	GENERATED_BODY()
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
	float MaxAir = 1.0f;


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

	bool FindDataTableByItemType(UWorld* World)
		override { return BaseFindDataTableByItemType<UBreathingTankItem, FBreathingTankItemTableRowInfo>(World); }

	const FItemTableRowInfoBase* GetItemStaticInfo()
		override { return ItemTableRowInfo.Get(); }

	UItemDynamicInfo* GetItemDynamicInfo() 
		override { return ItemDynamicInfo.Get(); }

	void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) override;

	float GetBreathingTankMaxAir() { return ItemTableRowInfo->MaxAir; }

};
