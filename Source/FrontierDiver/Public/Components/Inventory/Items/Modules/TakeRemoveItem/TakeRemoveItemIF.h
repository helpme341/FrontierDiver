// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/Items/Modules/ItemIFBase.h"
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
class FRONTIERDIVER_API ITakeRemoveItemIF : public IItemIFBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	const virtual FName GetSocketNameForItem() =0;

	const virtual EAnimItemBlendType GetAnimItemBlendType() = 0;

	virtual bool UseStaticMesh() =0;

	virtual void SetHeldMeshItem(AStaticMeshActor* HeldMesh) =0;

	virtual AStaticMeshActor* GetHeldMeshItem() =0;

	const virtual FTransform GetHeldMeshItemOffset() =0;

	virtual void OnTakeItem(UInventoryComponent* Inventory) =0;

	virtual void OnRemoveItem(UInventoryComponent* Inventory) =0;
};
