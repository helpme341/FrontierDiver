// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/Items/Modules/TakeRemoveItem/TakeRemoveItemIF.h"
#include "Engine/StaticMeshActor.h"
#include "Components/Inventory/Items/Modules/InteractItemIF.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

bool UInventoryComponent::AddItemToInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->bUseCustomAddThisItemToInventory)
    {
        if (Item && Item->AddThisItemToInventory(this)) { return true; }
        UE_LOG(LogInventoryComponent, Warning, TEXT("Custom AddThisItemToInventory failed or Item is nullptr"));
        return false;
    }
    else
    {
        return BaseAddItemToInventory(Item, DestroyItem);
    }
}

bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->bUseCustomRemoveThisItemFromInventory)
    {
        if (Item)
        { 
            if (Item->RemoveThisItemFromInventory(this, DestroyItem)) 
            {
                return true;
            }
        }
        UE_LOG(LogInventoryComponent, Warning, TEXT("Custom RemoveThisItemFromInventory failed or Item is nullptr"));
        return false;
    }
    else
    {
        return BaseRemoveItemFromInventory(Item, DestroyItem);
    }
}

bool UInventoryComponent::PickupItemToInventory(AWorldItem* Item)
{
    UItemBase* NewItem = NewObject<UItemBase>(this, Item->ItemType);
    if (NewItem)
    {
        IPickupDropItemIF* PickupDropItemIF = Cast<IPickupDropItemIF>(NewItem);
        if (PickupDropItemIF)
        {
            if (PickupDropItemIF->PickupItem(this, Item))
            {
                if (NewItem->GetItemDynamicInfo().QuantityItems == 0)
                {
                    NewItem->ConditionalBeginDestroy();
                }
                return true;
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to cast NewItem to IPickupDropItemIF or NewItem is nullptr"));
        }
        NewItem->ConditionalBeginDestroy();
    }
    else
    {
        UE_LOG(LogInventoryComponent, Error, TEXT("Failed to create NewItem"));
    }
    return false;
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
    if (IPickupDropItemIF* PickupDropItemIF = Cast<IPickupDropItemIF>(Item))
    {
        if (PickupDropItemIF->DropItem(this))
        {
            if (Item->GetItemDynamicInfo().QuantityItems == 0) { Item->ConditionalBeginDestroy(); }
            return true;
        }
    }
    UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to cast Item to IPickupDropItemIF or Item is nullptr"));
    return false;
}

bool UInventoryComponent::ThirdInteractWithHeldItem()
{
    if (bIsItemHeld)
    {
        IInteractItemIF* InteractItemIF = Cast<IInteractItemIF>(HeldItem);
        if (InteractItemIF->_getUObject()->IsValidLowLevel()) { InteractItemIF->ThirdInteract(this); }
        return true;
    }
    return false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    if (InventoryWidgetClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);

            if (InventoryWidget)
            {
                InventoryWidget->InventoryComponent = this;
                InventoryWidget->LoadWidgestSlots();
                InventoryWidget->CreateWidgets();
                InventoryWidget->UpdateAllWidgets();
                InventoryWidget->AddToViewport(0);
            }
            else
            {
                UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to create InventoryWidget"));
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("PlayerController not found"));
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("InventoryWidgetClass is nullptr"));
    }
}

bool UInventoryComponent::BaseAddItemToInventory(UItemBase* Item, bool DestroyItem)
{
    if (!Item || !Item->IsValidLowLevel() || Item->ThisItemID != 99)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or ThisItemID is not 99"));
        return false;
    }
    if (Item->FindDataTableByItemType())
    {
        const FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
        if (ItemTableRowInfo && Inventory.Contains(ItemTableRowInfo->ItemContainerType))
        {
            TArray<UItemBase*>& ContainerInventory = Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory;
            if (!ContainerInventory.IsEmpty())
            {
                bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;
                if (Item->GetItemDynamicInfo().ItemTypeName != "None" && Item->GetItemDynamicInfo().QuantityItems != 0)
                {
                    for (int32 Counter = 0; Counter < ContainerInventory.Num(); Counter++)
                    {
                        UItemBase* ItemOnInspection = ContainerInventory[Counter];

                        if (bCheckQuantity && ItemOnInspection)
                        {

                            if (ItemOnInspection->GetItemDynamicInfo().ItemTypeName == Item->GetItemDynamicInfo().ItemTypeName)
                            {
                                if (ItemOnInspection->GetItemDynamicInfo().QuantityItems + Item->GetItemDynamicInfo().QuantityItems <= ItemTableRowInfo->MaxQuantityItemsInSlot)
                                {
                                    ItemOnInspection->GetItemDynamicInfo().QuantityItems += Item->GetItemDynamicInfo().QuantityItems;
                                    Item->GetItemDynamicInfo().QuantityItems = 0;
                                    InventoryWidget->UpdateWidgetByItem(ItemOnInspection, false);
                                    return true;
                                }
                            }
                        }

                        if (!ItemOnInspection)
                        {
                            ContainerInventory[Counter] = Item;
                            Item->ThisItemID = Counter;
                            InventoryWidget->UpdateWidgetByItem(Item, false);
                            return true;
                        }
                    }
                }
                else
                {
                    UE_LOG(LogInventoryComponent, Warning, TEXT("ItemDynamicInfo is nullptr"));
                }
            }
            else
            {
                UE_LOG(LogInventoryComponent, Warning, TEXT("ContainerInventory is empty"));
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("ItemTableRowInfo is nullptr or ItemContainerType not found in Inventory"));
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
    }
    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
    return false;
}

bool UInventoryComponent::BaseRemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
    if (Item->ThisItemID != 99 && Item)
    {
        if (Item->FindDataTableByItemType())
        {
            const FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
            if (ItemTableRowInfo)
            {
                if (ItemTableRowInfo->MaxQuantityItemsInSlot > 1 && Item->GetItemDynamicInfo().QuantityItems - 1 >= 1)
                {
                    Item->GetItemDynamicInfo().QuantityItems--;
                    InventoryWidget->UpdateWidgetByItem(Item, false);
                    return true;
                }
                else if (Inventory.Contains(ItemTableRowInfo->ItemContainerType))
                {
                    Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Item->ThisItemID] = nullptr;
                    Item->GetItemDynamicInfo().QuantityItems = 0;
                    InventoryWidget->UpdateWidgetByItem(Item, true);
                    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
                    else { Item->ThisItemID = 99; }
                    return true;
                }
            }
            else
            {
                UE_LOG(LogInventoryComponent, Warning, TEXT("ItemTableRowInfo or ItemDynamicInfo is nullptr"));
            }
        }
        else
        {
            UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or ThisItemID is 99"));
    }
    return false;
}

bool UInventoryComponent::TakeItemToHandsByID(int32 ID)
{
    if (!bIsItemHeld && Inventory[QuickInventoryContainerType].ContainerInventory[ID])
    {
        ITakeRemoveItemIF* TakeRemoveItem = Cast<ITakeRemoveItemIF>(Inventory[QuickInventoryContainerType].ContainerInventory[ID]);
        if (TakeRemoveItem)
        {
            if (TakeRemoveItem->UseStaticMesh() && TakeRemoveItem->GetSocketNameForItem() != "None")
            {
                AStaticMeshActor* HeldMeshItem = GetWorld()->SpawnActor<AStaticMeshActor>();
                if (HeldMeshItem)
                {
                    TakeRemoveItem->SetHeldMeshItem(HeldMeshItem);

                    // Установите подвижность для HeldMeshItem
                    HeldMeshItem->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
                    HeldMeshItem->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

                    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
                    HeldMeshItem->AttachToComponent(GetOwnerCharacter()->Mesh1P, AttachmentRules, TakeRemoveItem->GetSocketNameForItem());
                    HeldMeshItem->AddActorLocalTransform(TakeRemoveItem->GetHeldMeshItemOffset());

                    HeldItem = Inventory[QuickInventoryContainerType].ContainerInventory[ID];

                    if (HeldMeshItem->GetStaticMeshComponent() && HeldItem->GetItemStaticInfo()->ItemWorldStaticMesh)
                    {
                        HeldMeshItem->GetStaticMeshComponent()->SetStaticMesh(HeldItem->GetItemStaticInfo()->ItemWorldStaticMesh);
                    }

                    TakeRemoveItem->OnTakeItem(this);
                    GetOwnerCharacter()->AnimItemBlendTypeNow = TakeRemoveItem->GetAnimItemBlendType();
                    bIsItemHeld = true;
                    return true;
                }
            }
            else
            {
                // логика с скелет мешем 
            }
        }
    }
    return false;
}

bool UInventoryComponent::RemoveItemFromHands()
{
    if (bIsItemHeld)
    {
        ITakeRemoveItemIF* TakeRemoveItem = Cast<ITakeRemoveItemIF>(HeldItem);
        if (TakeRemoveItem)
        {
            if (TakeRemoveItem->UseStaticMesh())
            {
                TakeRemoveItem->GetHeldMeshItem()->Destroy();
                GetOwnerCharacter()->AnimItemBlendTypeNow = EAnimItemBlendType::None;
                HeldItem = nullptr;
                bIsItemHeld = false;
                TakeRemoveItem->OnRemoveItem(this);
                return true;
            }
            else
            {
                // логика с скелет мешем 
            }
        }

    }
    return false;
}