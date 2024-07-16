// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/Items/Interfaces/TakeRemoveItemIF.h"
#include "Components/Inventory/Items/Interfaces/InteractItemIF.h"
#include "Engine/StaticMeshActor.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

bool UInventoryComponent::AddItemToInventory(UItemBase* Item, bool DestroyItem)
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

                            if (ItemOnInspection->GetItemDynamicInfo().ItemTypeName == Item->GetItemDynamicInfo().ItemTypeName && ItemOnInspection->GetClass() == Item->GetClass())
                            {
                                if (ItemOnInspection->GetItemDynamicInfo().QuantityItems + Item->GetItemDynamicInfo().QuantityItems <= ItemTableRowInfo->MaxQuantityItemsInSlot)
                                {
                                    ItemOnInspection->GetItemDynamicInfo().QuantityItems += Item->GetItemDynamicInfo().QuantityItems;
                                    Item->GetItemDynamicInfo().QuantityItems = 0;
                                    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
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
    return false;
}

bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
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

bool UInventoryComponent::PickupItemToInventory(AWorldItem* Item)
{
    UItemBase* NewItem = NewObject<UItemBase>(this, Item->ItemType);
    if (NewItem)
    {
        if (Item->ItemTypeName != "None")
        {
            NewItem->GetItemDynamicInfo().ItemTypeName = Item->ItemTypeName;
        }
        else if (Item->ItemDynamicInfo.ItemTypeName != "None")
        {
            NewItem->GetItemDynamicInfo() = Item->ItemDynamicInfo;
        }
        else
        {
            NewItem->ConditionalBeginDestroy();
            return false;
        }

        if (AddItemToInventory(NewItem, true))
        {
            Item->Destroy();
            return true;
        }
        else
        {
            NewItem->ConditionalBeginDestroy();
            return false;
        }
    }
    else
    {
        NewItem->ConditionalBeginDestroy();
    }
    return false;
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
    if (Item->bIsPlayerCanDropThisItem)
    {
        AWorldItem* NewWorldItem = Item->GetWorld()->SpawnActor<AWorldItem>();
        if (NewWorldItem)
        {
            if (Item->FindDataTableByItemType())
            {
                if (RemoveItemFromInventory(Item, false))
                {
                    FItemDynamicInfoBase ItemDynamic = Item->GetItemDynamicInfo();
                    ItemDynamic.QuantityItems = 1;
                    NewWorldItem->LoadDataToWorldItem(ItemDynamic, Item->GetItemStaticInfo(), Item->GetClass());
                    NewWorldItem->SetActorLocation(GetOwnerCharacter()->GetActorLocation() + GetOwnerCharacter()->GetActorForwardVector() * PlayerDropLocationOffset);
                    if (Item->GetItemDynamicInfo().QuantityItems == 0) { Item->ConditionalBeginDestroy(); }
                    return true;
                }
            }
        }
        NewWorldItem->Destroy();
        return false;
    }
    return false;
}

bool UInventoryComponent::FirstInteractWithHeldItem()
{
    if (bIsItemHeld)
    {
        IInteractItemIF* InteractItemIF = Cast<IInteractItemIF>(HeldItem);
        if (InteractItemIF->_getUObject()->IsValidLowLevel()) { InteractItemIF->FirstInteract(this); }
        return true;
    }
    return false;
}

bool UInventoryComponent::SecondInteractWithHeldItem()
{
    if (bIsItemHeld)
    {
        IInteractItemIF* InteractItemIF = Cast<IInteractItemIF>(HeldItem);
        if (InteractItemIF->_getUObject()->IsValidLowLevel()) { InteractItemIF->SecondInteract(this); }
        return true;
    }
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

bool UInventoryComponent::TakeItemToHandsByID(int32 ID)
{
    if (!bIsItemHeld && Inventory[QuickInventoryContainerType].ContainerInventory[ID])
    {
        ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(Inventory[QuickInventoryContainerType].ContainerInventory[ID]);
        if (TakeRemoveItemIF)
        {
            const FHeldItemInfo& HeldItemInfo = TakeRemoveItemIF->GetHeldItemInfo();
            if (HeldItemInfo.ItemSocketName != "None" && HeldItemInfo.ItemAnimBlendType != EAnimItemBlendType::None)
            {
                if (TakeRemoveItemIF->UseStaticMesh())
                {
                    AStaticMeshActor* HeldMeshItem = GetWorld()->SpawnActor<AStaticMeshActor>();
                    if (HeldMeshItem)
                    {
                        TakeRemoveItemIF->SetHeldMeshItem(HeldMeshItem);

                        // Установите подвижность для HeldMeshItem
                        HeldMeshItem->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
                        HeldMeshItem->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

                        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
                        HeldMeshItem->AttachToComponent(GetOwnerCharacter()->Mesh1P, AttachmentRules, HeldItemInfo.ItemSocketName);
                        HeldMeshItem->AddActorLocalTransform(HeldItemInfo.ItemItemAttachOffset);


                        HeldItem = Inventory[QuickInventoryContainerType].ContainerInventory[ID];

                        HeldMeshItem->SetActorScale3D(HeldItem->GetItemStaticInfo()->WorldItemScale);

                        if (HeldMeshItem->GetStaticMeshComponent() && HeldItem->GetItemStaticInfo()->WorldItemStaticMesh)
                        {
                            HeldMeshItem->GetStaticMeshComponent()->SetStaticMesh(HeldItem->GetItemStaticInfo()->WorldItemStaticMesh);
                        }

                        TakeRemoveItemIF->OnTakeItem(this);
                        GetOwnerCharacter()->AnimItemBlendTypeNow = HeldItemInfo.ItemAnimBlendType;
                        bIsItemHeld = true;
                        return true;
                    }
                    else { HeldMeshItem->Destroy(); }
                }
                else
                {
                    // логика с скелет мешем 
                }
            }
        }
    }
    return false;
}

bool UInventoryComponent::RemoveItemFromHands()
{
    if (bIsItemHeld)
    {
        ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(HeldItem);
        if (TakeRemoveItemIF)
        {
            if (TakeRemoveItemIF->UseStaticMesh())
            {
                TakeRemoveItemIF->GetHeldMeshItem()->Destroy();
                GetOwnerCharacter()->AnimItemBlendTypeNow = EAnimItemBlendType::None;
                HeldItem = nullptr;
                bIsItemHeld = false;
                TakeRemoveItemIF->OnRemoveItem(this);
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