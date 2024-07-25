// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/BreathingTankItem.h"

UBreathingTankItem::UBreathingTankItem()
{
	bIsPlayerCanDropThisItem = true;

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
