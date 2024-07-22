// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/Interfaces/InteractItemIF.h"
#include "Components/Inventory/Items/Interfaces/TakeRemoveItemIF.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Character/FrontierDiverCharacter.h"
#include "Engine/SpotLight.h"
#include "FlashlightItem.generated.h"

UCLASS()
class FRONTIERDIVER_API UFlashlightItemDynamicInfo : public UItemDynamicInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlashlightSettings")
	bool bIsFlashlightOn;

	TStrongObjectPtr<ASpotLight> SpotLight;
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
	FHeldItemInfo HeldItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings|SocketNames")
	FName ItemLightSocketName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings|AttachOffset")
	FTransform SpotLightAttachOffset;
};

class AWorldItem;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UFlashlightItem :
	public UItemBase,
	public TItemTmpl<UFlashlightItemDynamicInfo, FFlashlightItemTableRowInfo>,
	public ITakeRemoveItemIF,
	public IInteractItemIF
{
	GENERATED_BODY()

public:

	UFlashlightItem();

	TStrongObjectPtr<AStaticMeshActor> HeldMeshItem;

	bool FindDataTableByItemType(UWorld* World)
		override { return BaseFindDataTableByItemType<UFlashlightItem, FFlashlightItemTableRowInfo>(World); }

	const FItemTableRowInfoBase* GetItemStaticInfo()
		override { return ItemTableRowInfo.Get(); }

	UItemDynamicInfo* GetItemDynamicInfo() 
		override { return ItemDynamicInfo.Get(); }

	void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) override;

	bool UseStaticMesh()
		override { return true; }

	void SetHeldMeshItem(AStaticMeshActor* HeldMesh)
		override { HeldMeshItem.Reset(HeldMesh); }

	AStaticMeshActor* GetHeldMeshItem()
		override { return  HeldMeshItem.Get(); }

	bool CanDrop()
		override { return true;};

	const struct FHeldItemInfo* GetHeldItemInfo() override;

	void ThirdInteract(UInventoryComponent* Inventory) override;

	void OnDropItem(AWorldItem* WorldItem) override;

	void OnTakeItem(UInventoryComponent* Inventory) override;

	void OnPickupItemToInventory(AWorldItem* Item) override;

    void OnRemoveItem(UInventoryComponent* Inventory) override;
};
