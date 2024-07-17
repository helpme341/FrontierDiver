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

const FTransform& UFlashlightItem::GetWorldItemOffset()
{
    // TODO: вставьте здесь оператор return
}

const FHeldItemInfo& UFlashlightItem::GetHeldItemInfo()
{
    if (FindDataTableByItemType())
    {
        return ItemTableRowInfo->HeldItemInfo;
    }
    static const FHeldItemInfo DefaultHeldItemInfo = FHeldItemInfo();
    return DefaultHeldItemInfo;
}

void UFlashlightItem::ThirdInteract(UInventoryComponent* Inventory)
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
        ItemDynamicInfo->SpotLight = GetWorld()->SpawnActor<ASpotLight>();
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
        if (ItemDynamicInfo->SpotLight)
        {
            ItemDynamicInfo->SpotLight->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
            ItemDynamicInfo->SpotLight->AttachToComponent(WorldItem->StaticMesh, AttachmentRules, ItemTableRowInfo->ItemLightSocketName);
            ItemDynamicInfo->SpotLight->AddActorLocalTransform(ItemTableRowInfo->SpotLightAttachOffset);
        }
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
