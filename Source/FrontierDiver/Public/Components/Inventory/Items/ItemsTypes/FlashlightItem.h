// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"
#include "Components/Inventory/Items/Modules/InteractItemIF.h"
#include "Components/Inventory/Items/Modules/TakeRemoveItem/TakeRemoveItemIF.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIFTmplImpl.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Character/FrontierDiverCharacter.h"
#include "FlashlightItem.generated.h"

USTRUCT(BlueprintType)
struct FFlashlightItemDynamicInfo : public FItemDynamicInfoBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FFlashlightItemTableRowInfo : public FItemTableRowInfoBase
{
	GENERATED_BODY()
	 
	FFlashlightItemTableRowInfo()
	{
		MaxQuantityItemsInSlot = 1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings|SocketNames")
	FName ItemSocketName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings|SocketNames")
	FName ItemLightSocketName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings|AttachOffset")
	FTransform  MeshItemAttachOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings|AttachOffset")
	FTransform SpotLightAttachOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings")
	EAnimItemBlendType ItemAnimBlendType;
};


/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UFlashlightItem :
	public UItemBase,
	public TItemTmpl<UFlashlightItem, FFlashlightItemDynamicInfo, FFlashlightItemTableRowInfo>,
	public IPickupDropItemIF,
	public IInteractionIF,
	public ITakeRemoveItemIF,
	public IInteractItemIF,
	public TPickupDropItemIFTmplImpl<UFlashlightItem>
{
	GENERATED_BODY()

public:
	UFlashlightItem();

	bool bIsFlashlightOff = true;

	AStaticMeshActor* HeldMeshItem;

	class ASpotLight* SpotLight;

	bool PickupItem(UInventoryComponent* Inventory, AWorldItem* Item)
		override { return TPickupDropItemIFTmplImpl::PickupItem(Inventory, Item); }

	bool DropItem(UInventoryComponent* Inventory)
		override { return TPickupDropItemIFTmplImpl::DropItem(Inventory); }

	bool FindDataTableByItemType()
		override { return BaseFindDataTableByItemType<UFlashlightItem, FFlashlightItemTableRowInfo>(); }

	const FItemTableRowInfoBase* GetItemStaticInfo()
		override { return ItemTableRowInfo; }

	FItemDynamicInfoBase& GetItemDynamicInfo() 
		override { return ItemDynamicInfo; }

	bool UseStaticMesh()
		override { return true; }

	void SetHeldMeshItem(AStaticMeshActor* HeldMesh)
		override { HeldMeshItem = HeldMesh; }

	AStaticMeshActor* GetHeldMeshItem()
		override { return HeldMeshItem; }

	const FTransform  GetHeldMeshItemOffset()
		override { return ItemTableRowInfo->MeshItemAttachOffset; }

	const FName GetSocketNameForItem() override;

	const EAnimItemBlendType GetAnimItemBlendType() override;

	void ThirdInteract(class UInventoryComponent* Inventory) override;
};
