// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Base/CharacterComponentBase.h"
#include "Components/Inventory/InventoryInterfaces/MainInventory/MainInventoryCostumeInterface.h"
#include "Components/Inventory/InventoryInterfaces/InventoryCylinder/InventoryCylinderInterface.h"
#include "InventoryComponent.generated.h"

class AStaticMeshActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRONTIERDIVER_API UInventoryComponent : public UCharacterComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	TMap<EMainInventorySlotType, FItemInfoBase> MainInventory = {
		{EMainInventorySlotType::Helmet, FItemInfoBase()},
	    {EMainInventorySlotType::Costume,FItemInfoBase()},
	   {EMainInventorySlotType::Sheets, FItemInfoBase()},
	};

	bool AddItemToInventory(FItemInfoBase ItemInfo);

	//void RemoveItemFromInventory(EInventorySlotType InventorySlotType);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;	
};
