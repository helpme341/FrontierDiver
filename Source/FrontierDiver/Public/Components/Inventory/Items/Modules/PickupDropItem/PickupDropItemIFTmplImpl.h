// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/ItemTmpl.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/FrontierDiverCharacter.h"
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
    if (Item->ItemTypeName != "None")
    {
        Owner->ItemDynamicInfo.ItemTypeName = Item->ItemTypeName;
    }
    else if (Item->ItemDynamicInfo.ItemTypeName != "None")
    {
        Owner->ItemDynamicInfo.ItemTypeName = Item->ItemDynamicInfo.ItemTypeName;
    }
    else
    {
        return false;
    }

    if (Inventory->AddItemToInventory(Owner, false))
    {
        Item->Destroy();
        return true;
    }
    return false;
}

template<typename OWT>
inline bool TPickupDropItemIFTmplImpl<OWT>::DropItem(UInventoryComponent* Inventory)
{
    // ���������, ��� NewWorldItem ��������� �������
    AWorldItem* NewWorldItem = Owner->GetWorld()->SpawnActor<AWorldItem>();
    if (!NewWorldItem)
    {
        return false; // ���� �������� �� �������, ������ ������� false
    }

    // ���������, ����� �� ��� �������� � ������� ������
    if (Owner->FindDataTableByItemType())
    {
        // ���������� ������� ������� �� ���������
        if (Inventory->RemoveItemFromInventory(Owner, false))
        {
            // ���� �������� ������ �������, ��������� ����� ������� �������
            FItemDynamicInfoBase ItemDynamic = Owner->GetItemDynamicInfo();
            ItemDynamic.QuantityItems = 1;
            NewWorldItem->LoadDataToWorldItem(ItemDynamic, OWT::StaticClass(), Owner->GetItemStaticInfo()->ItemWorldStaticMesh);
            NewWorldItem->SetActorLocation(Inventory->GetOwnerCharacter()->GetActorLocation() + Inventory->GetOwnerCharacter()->GetActorForwardVector() * Inventory->PlayerDropLocationOffset);
            return true;
        }
    }

    // ���������� NewWorldItem, ���� ���������� ���� �� �������
    NewWorldItem->Destroy();
    return false;
}

template<typename OWT>
inline TPickupDropItemIFTmplImpl<OWT> TPickupDropItemIFTmplImpl<OWT>::PickupDropIFTmplImpl()
{
	return this;
}
