// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/BreathingTankItem.h"
#include "Components/Image.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"

UBreathingTankItem::UBreathingTankItem()
{
	bIsPlayerCanDropThisItem = true;

    bUseCustomUpdateWidget = true;

    bUseCustomShowItemInfo = true;

    bUseCustomUpdateItemInfo = true;
}


void UBreathingTankItem::OnAddItemToInventory()
{
    if (ItemWidget.IsValid()) { ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ItemDynamicInfo->CurrentAir))); }
}

void UBreathingTankItem::SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo)
{
    if (!DynamicInfo)
    {
        UE_LOG(LogTemp, Warning, TEXT("DynamicInfo is nullptr!"));
        return;
    }

    UBreathingTankItemDynamicInfo* BreathingInfo = Cast< UBreathingTankItemDynamicInfo>(DynamicInfo);
    if (BreathingInfo)
    {
        ItemDynamicInfo.Reset(BreathingInfo);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to cast DynamicInfo to UFlashlightItemDynamicInfo!"));
    }
}

void UBreathingTankItem::CustomUpdateWidget()
{
    ItemWidget->WidgetImage->SetBrushFromTexture(ItemTableRowInfo->ItemWidgetTexture);
    ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ItemDynamicInfo->CurrentAir)));
}

void UBreathingTankItem::CustomShowItemInfo()
{
    ItemWidget->InventoryWidget->ItemImage->SetBrushFromTexture(ItemTableRowInfo->ItemWidgetTexture);
    ItemWidget->InventoryWidget->ItemDescription->SetText(ItemTableRowInfo->ItemDescription);
    ItemWidget->InventoryWidget->ItemNameTextBlock->SetText(FText::FromName(ItemDynamicInfo->ItemTypeName));

    FString QuantityString = FString::Printf(TEXT("%.1f/%.1f"), ItemDynamicInfo->CurrentAir, ItemTableRowInfo->MaxAir);

    ItemWidget->InventoryWidget->ItemQuantityTextBlock->SetText(FText::FromString(QuantityString));
}

void UBreathingTankItem::CustomUpdateItemInfo()
{
    ItemWidget->InventoryWidget->ItemImage->SetBrushFromTexture(ItemTableRowInfo->ItemWidgetTexture);
    ItemWidget->InventoryWidget->ItemDescription->SetText(ItemTableRowInfo->ItemDescription);
    ItemWidget->InventoryWidget->ItemNameTextBlock->SetText(FText::FromName(ItemDynamicInfo->ItemTypeName));

    FString QuantityString = FString::Printf(TEXT("%.1f/%.1f"), ItemDynamicInfo->CurrentAir, ItemTableRowInfo->MaxAir);

    ItemWidget->InventoryWidget->ItemQuantityTextBlock->SetText(FText::FromString(QuantityString));
}
