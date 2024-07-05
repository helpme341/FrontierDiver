// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/Modules/ItemTmplBase.h"



/**
 * 
 */
template<typename OWT>
class FRONTIERDIVER_API TPickupDropItemIFTmplImpl : public TItemTmplBase
{
protected:

	OWT* Owner;

public:


	bool PickupItem(UInventoryComponent* Inventory, AWorldItem* Item);

	bool DropItem(UInventoryComponent* Inventory);

	TPickupDropItemIFTmplImpl<OWT> PickupDropIFTmplImpl();
};

template<typename OWT>
inline bool TPickupDropItemIFTmplImpl<OWT>::PickupItem(UInventoryComponent* Inventory, AWorldItem* Item)
{
	if (auto* ItemTmpl = Owner->GetItemTmpl())
	{
		ItemTmpl->ItemDynamicInfo.ItemTypeName = Item->ItemTypeName;
		if (Inventory->AddItemToInventory(Owner))
		{
			Item->Destroy();
			return true;
		}
		Owner->ConditionalBeginDestroy();
	}

	return false;
}

template<typename OWT>
inline bool TPickupDropItemIFTmplImpl<OWT>::DropItem(UInventoryComponent* Inventory)
{
	AWorldItem* NewWorldItem = Owner->GetWorld()->SpawnActor<AWorldItem>();
	if (Owner->FindDataTableByItemType(Inventory))
	{
		auto* ItemTmpl = Owner->GetItemTmpl();
		if (ItemTmpl)
		{
			NewWorldItem->LoadDataToWorldItem(ItemTmpl->ItemDynamicInfo, OWT::StaticClass(), ItemTmpl->ItemTableRowInfo->ItemWorldStaticMesh);
			return Inventory->RemoveItemFromInventory(Owner, true);
		}
	}
	return false;
}

template<typename OWT>
inline TPickupDropItemIFTmplImpl<OWT> TPickupDropItemIFTmplImpl<OWT>::PickupDropIFTmplImpl()
{
	return this;
}
