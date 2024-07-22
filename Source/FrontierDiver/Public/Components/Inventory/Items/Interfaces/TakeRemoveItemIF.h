// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/FrontierDiverCharacter.h"
#include "TakeRemoveItemIF.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTakeRemoveItemIF : public UInterface
{
	GENERATED_BODY()
};


class UInventoryComponent;

/**
 * 
 */
class FRONTIERDIVER_API ITakeRemoveItemIF : public IInterface
{
	GENERATED_BODY()

public:

	virtual bool UseStaticMesh() =0;

	virtual void SetHeldMeshItem(AStaticMeshActor* HeldMesh) =0;

	virtual AStaticMeshActor* GetHeldMeshItem() =0;

	virtual const struct FHeldItemInfo* GetHeldItemInfo() =0;

	virtual bool CanDrop() = 0;

	virtual void OnDropItem(class AWorldItem* WorldItem) {};

	virtual void OnTakeItem(UInventoryComponent* Inventory) =0;

	virtual void OnRemoveItem(UInventoryComponent* Inventory) =0;
};
