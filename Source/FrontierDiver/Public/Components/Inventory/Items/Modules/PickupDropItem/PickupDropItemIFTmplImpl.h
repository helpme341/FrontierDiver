// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Components/Inventory/Items/Modules/ItemTmplBase.h"

class UInventoryComponent;

/**
 * 
 */
template<typename OWT, typename DST, typename TRST>
class FRONTIERDIVER_API TPickupDropItemIFTmplImpl : public TItemTmplBase, public IPickupDropItemIF
{
protected:

	OWT* Owner;

public:


	bool PickupItem(UInventoryComponent* Inventory, AWorldItem* Item) override;

	bool DropItem(UInventoryComponent* Inventory) override;

	template<typename T>
	T PickupDropIFTmplImpl();
};

template<typename OWT, typename DST, typename TRST>
inline bool TPickupDropItemIFTmplImpl<OWT, DST, TRST>::PickupItem(UInventoryComponent* Inventory, AWorldItem* Item)
{
	Owner->GetItemTmpl()->ItemDynamicInfo.ItemTypeName = Item->ItemDynamicInfo.ItemTypeName;
	if (Owner->AddThisItemToInventory(Inventory))
	{
		Item->Destroy();
		return true;
	}
	Owner->ConditionalBeginDestroy();
	return false;
}

template<typename OWT, typename DST, typename TRST>
inline bool TPickupDropItemIFTmplImpl<OWT, DST, TRST>::DropItem(UInventoryComponent* Inventory)
{
	AWorldItem* NewWorldItem = Owner->GetWorld()->SpawnActor<AWorldItem>();
	if (Owner->FindDataTableByItemType(Inventory))
	{
		auto* ItemTmpl = Owner->GetItemTmpl();
		if (ItemTmpl)
		{
			NewWorldItem->LoadDataToWorldItem(ItemTmpl->ItemDynamicInfo, OWT::StaticClass(), ItemTmpl->ItemTableRowInfo->ItemWorldStaticMesh);
			return Owner->RemoveThisItemFromInventory(Inventory, true);
		}
	}
	return false;
}

template<typename OWT, typename DST, typename TRST>
template<typename T>
inline T TPickupDropItemIFTmplImpl<OWT, DST, TRST>::PickupDropIFTmplImpl()
{
	return this;
}
