// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemsTypes/FlashlightItem.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMeshActor.h"


UFlashlightItem::UFlashlightItem()
{
	TItemTmpl::Owner = this;
	TPickupDropItemIFTmplImpl::Owner = this;
}

const FName UFlashlightItem::GetSocketNameForItem()
{
	if (FindDataTableByItemType())
	{
		return ItemTableRowInfo->ItemSocketName;
	}
	return "None";
}

const EAnimItemBlendType UFlashlightItem::GetAnimItemBlendType()
{
	if (FindDataTableByItemType())
	{
		return ItemTableRowInfo->ItemAnimBlendType;
	}
	return EAnimItemBlendType::None;
}

void UFlashlightItem::MainInteract(UInventoryComponent* Inventory)
{
	if (bIsFlashlightOff)
	{
		SpotLight->Destroy();
	}
	else
	{
		SpotLight = Inventory->GetWorld()->SpawnActor<ASpotLight>();
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		SpotLight->AttachToComponent(HeldMeshItem->GetStaticMeshComponent(), AttachmentRules, ItemTableRowInfo->ItemLightSocketName);
		SpotLight->AddActorLocalTransform(ItemTableRowInfo->SpotLightAttachOffset);
	}
}