// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/FlashlightItem.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/Inventory/Items/WorldItem.h"

UFlashlightItem::UFlashlightItem()
{
    bIsPlayerCanDropThisItem = true;
}

const FHeldItemInfo* UFlashlightItem::GetHeldItemInfo()
{
    if (FindDataTableByItemType(GetWorld()))
    {
        return &ItemTableRowInfo->HeldItemInfo;
    }
    return nullptr;
}

void UFlashlightItem::SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo)
{
    if (!DynamicInfo)
    {
        UE_LOG(LogTemp, Warning, TEXT("DynamicInfo is nullptr!"));
        return;
    }

    UFlashlightItemDynamicInfo* FlashlightInfo = Cast<UFlashlightItemDynamicInfo>(DynamicInfo);
    if (FlashlightInfo)
    {
        ItemDynamicInfo.Reset(FlashlightInfo);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to cast DynamicInfo to UFlashlightItemDynamicInfo!"));
    }
}

void UFlashlightItem::ThirdInteract(UInventoryComponent* Inventory)
{
    if (ItemDynamicInfo)
    {
        if (ItemDynamicInfo->ItemTypeName != "None" && ItemDynamicInfo->QuantityItems >= 0)
        {
            if (ItemDynamicInfo->SpotLight)
            {
                if (ItemDynamicInfo->bIsFlashlightOn)
                {
                    ItemDynamicInfo->SpotLight->SetActorHiddenInGame(true);
                    ItemDynamicInfo->bIsFlashlightOn = false;
                }
                else if (!ItemDynamicInfo->bIsFlashlightOn)
                {
                    ItemDynamicInfo->SpotLight->SetActorHiddenInGame(false);
                    ItemDynamicInfo->bIsFlashlightOn = true;
                }
            }
        }
    }
}

void UFlashlightItem::OnPickupItemToInventory(AWorldItem* Item)
{
    if (ItemDynamicInfo->SpotLight)
    {
        ItemDynamicInfo->SpotLight->Destroy();
        ItemDynamicInfo->SpotLight = nullptr;
    }
}

void UFlashlightItem::OnTakeItem(UInventoryComponent* Inventory)
{
    if (!ItemDynamicInfo->SpotLight)
    {
        ItemDynamicInfo->SpotLight.Reset(GetWorld()->SpawnActor<ASpotLight>());
        if (ItemDynamicInfo->SpotLight)
        {
            ItemDynamicInfo->SpotLight->SetMobility(EComponentMobility::Movable);
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
            ItemDynamicInfo->SpotLight->AttachToComponent(HeldMeshItem->GetStaticMeshComponent(), AttachmentRules, ItemTableRowInfo->ItemLightSocketName);
            ItemDynamicInfo->SpotLight->AddActorLocalTransform(ItemTableRowInfo->SpotLightAttachOffset);
        }
    }
    else
    {
        ItemDynamicInfo->SpotLight->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        ItemDynamicInfo->SpotLight->AttachToComponent(HeldMeshItem->GetStaticMeshComponent(), AttachmentRules, ItemTableRowInfo->ItemLightSocketName);
        ItemDynamicInfo->SpotLight->AddActorLocalTransform(ItemTableRowInfo->SpotLightAttachOffset);
    }
    
    if (ItemDynamicInfo->SpotLight)
    {
        if (!ItemDynamicInfo->bIsFlashlightOn)
        {
            ItemDynamicInfo->SpotLight->SetActorHiddenInGame(true);
            ItemDynamicInfo->bIsFlashlightOn = false;
        }
        else if (ItemDynamicInfo->bIsFlashlightOn)
        {
            ItemDynamicInfo->SpotLight->SetActorHiddenInGame(false);
            ItemDynamicInfo->bIsFlashlightOn = true;
        }
    }
}

void UFlashlightItem::OnDropItem(AWorldItem* WorldItem)
{
    if (ItemDynamicInfo->bIsFlashlightOn)
    {
        if (!ItemDynamicInfo->SpotLight) { ItemDynamicInfo->SpotLight.Reset(GetWorld()->SpawnActor<ASpotLight>()); }
        ItemDynamicInfo->SpotLight->SetMobility(EComponentMobility::Movable);
        ItemDynamicInfo->SpotLight->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        ItemDynamicInfo->SpotLight->AttachToComponent(WorldItem->StaticMesh, AttachmentRules, ItemTableRowInfo->ItemLightSocketName);
        ItemDynamicInfo->SpotLight->AddActorLocalTransform(ItemTableRowInfo->SpotLightAttachOffset);
    }
    else if (ItemDynamicInfo->SpotLight)
    {
        ItemDynamicInfo->SpotLight->Destroy();
    }
}

void UFlashlightItem::OnRemoveItem(UInventoryComponent* Inventory)
{
    if (ItemDynamicInfo->SpotLight)
    {
        ItemDynamicInfo->SpotLight->Destroy();
        ItemDynamicInfo->SpotLight = nullptr;
    }
}