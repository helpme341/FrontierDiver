// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/BreathingTankItem.h"
#include "Components/TextBlock.h"

UBreathingTankItem::UBreathingTankItem()
{
	bIsPlayerCanDropThisItem = true;
}


void UBreathingTankItem::OnAddItemToInventory()
{
    if (ItemWidget.IsValid()) { ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), ItemDynamicInfo->CurrentAir))); }
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
