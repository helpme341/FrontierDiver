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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlashlightSettings")
	ASpotLight* SpotLight;
};

USTRUCT(BlueprintType)
struct FFlashlightItemTableRowInfo : public FItemTableRowInfoBase
{
	GENERATED_BODY()
	 
	FFlashlightItemTableRowInfo()
	{
		MaxQuantityItemsInSlot = 1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomFlashlightSettings")
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

	AStaticMeshActor* HeldMeshItem;

	bool FindDataTableByItemType(UWorld* World)
		override { return BaseFindDataTableByItemType<UFlashlightItem, FFlashlightItemTableRowInfo>(World); }

	const FItemTableRowInfoBase* GetItemStaticInfo()
		override { return ItemTableRowInfo; }


	UItemDynamicInfo* GetItemDynamicInfo() 
		override { return ItemDynamicInfo; }

	void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) override
	{
		// Проверка указателя на null
		if (!DynamicInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("DynamicInfo is nullptr!"));
			return;
		}

		// Выполнение каста и проверка успешности
		UFlashlightItemDynamicInfo* FlashlightInfo = Cast<UFlashlightItemDynamicInfo>(DynamicInfo);
		if (FlashlightInfo)
		{
			// Установка нового значения
			ItemDynamicInfo = FlashlightInfo;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast DynamicInfo to UFlashlightItemDynamicInfo!"));
		}
	}

	bool UseStaticMesh()
		override { return true; }

	void SetHeldMeshItem(AStaticMeshActor* HeldMesh)
		override { HeldMeshItem = HeldMesh; }

	AStaticMeshActor* GetHeldMeshItem()
		override { return  HeldMeshItem; }

	bool CanDrop()
		override { return true;};

	const FHeldItemInfo& GetHeldItemInfo() override;

	void ThirdInteract(UInventoryComponent* Inventory) override;

	void OnDropItem(AWorldItem* WorldItem) override;

	void OnTakeItem(UInventoryComponent* Inventory) override;

	void OnPickupItemToInventory(AWorldItem* Item) override;

    void OnRemoveItem(UInventoryComponent* Inventory) override;
};
