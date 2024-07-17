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
    if (!Item || Item->ThisItemID != 99)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or ThisItemID is not 99"));
        return false;
    }
    if (!Item->FindDataTableByItemType())
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
        return false;
    }

    const FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
    if (!ItemTableRowInfo || !Inventory.Contains(ItemTableRowInfo->ItemContainerType))
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("ItemTableRowInfo is nullptr or ItemContainerType not found in Inventory"));
        return false;
    }

    TArray<UItemBase*>& ContainerInventory = Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory;
    if (ContainerInventory.IsEmpty())
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("ContainerInventory is empty"));
        return false;
    }

    bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;
    if (Item->GetItemDynamicInfo() && Item->GetItemDynamicInfo()->ItemTypeName != "None" && Item->GetItemDynamicInfo()->QuantityItems != 0)
    {
        for (int32 Counter = 0; Counter < ContainerInventory.Num(); Counter++)
        {
            UItemBase* ItemOnInspection = ContainerInventory[Counter];

            if (bCheckQuantity && ItemOnInspection && ItemOnInspection->GetItemDynamicInfo()->ItemTypeName == Item->GetItemDynamicInfo()->ItemTypeName && ItemOnInspection->GetClass() == Item->GetClass())
            {
                if (ItemOnInspection->GetItemDynamicInfo()->QuantityItems + Item->GetItemDynamicInfo()->QuantityItems <= ItemTableRowInfo->MaxQuantityItemsInSlot)
                {
                    ItemOnInspection->GetItemDynamicInfo()->QuantityItems += Item->GetItemDynamicInfo()->QuantityItems;
                    Item->GetItemDynamicInfo()->QuantityItems = 0;
                    if (DestroyItem) { Item->ConditionalBeginDestroy(); }
                    InventoryWidget->UpdateWidgetByItem(ItemOnInspection, false);
                    return true;
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
        UE_LOG(LogInventoryComponent, Warning, TEXT("ItemDynamicInfo is nullptr or invalid"));
    }

    return false;
}

bool UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool DestroyItem)
{
    if (!Item || Item->ThisItemID == 99)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or ThisItemID is 99"));
        return false;
    }

    if (!Item->FindDataTableByItemType())
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
        return false;
    }

    const FItemTableRowInfoBase* ItemTableRowInfo = Item->GetItemStaticInfo();
    if (!ItemTableRowInfo)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("ItemTableRowInfo is nullptr"));
        return false;
    }

    if (ItemTableRowInfo->MaxQuantityItemsInSlot > 1 && Item->GetItemDynamicInfo()->QuantityItems > 1)
    {
        Item->GetItemDynamicInfo()->QuantityItems--;
        InventoryWidget->UpdateWidgetByItem(Item, false);
        return true;
    }
    else if (Inventory.Contains(ItemTableRowInfo->ItemContainerType))
    {
        Inventory[ItemTableRowInfo->ItemContainerType].ContainerInventory[Item->ThisItemID] = nullptr;
        InventoryWidget->UpdateWidgetByItem(Item, true);
        if (DestroyItem)
        {
            Item->ConditionalBeginDestroy();
        }
        else
        {
            Item->ThisItemID = 99;
        }
        return true;
    }

    return false;
}

bool UInventoryComponent::PickupItemToInventory(AWorldItem* Item)
{
    if (!Item) return false;

    UItemBase* NewItem = NewObject<UItemBase>(this, Item->ItemType);
    if (!NewItem)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to create NewItem"));
        return false;
    }

    if (Item->ItemDynamicInfo)
    {
        if (NewItem->GetItemDynamicInfo())
        {
            NewItem->GetItemDynamicInfo()->ConditionalBeginDestroy();
        }
        NewItem->SetItemDynamicInfo(Item->ItemDynamicInfo);
    }
    else if (Item->ItemTypeName != "None")
    {
        NewItem->GetItemDynamicInfo()->ItemTypeName = Item->ItemTypeName;
    }
    else
    {
        NewItem->ConditionalBeginDestroy();
        return false;
    }

    if (AddItemToInventory(NewItem, true))
    {
        NewItem->OnPickupItemToInventory(Item);
        Item->Destroy();
        return true;
    }
    else
    {
        NewItem->ConditionalBeginDestroy();
        return false;
    }
}

bool UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
    if (!Item || !Item->bIsPlayerCanDropThisItem)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or cannot be dropped"));
        return false;
    }

    AWorldItem* NewWorldItem = Item->GetWorld()->SpawnActor<AWorldItem>();
    if (!NewWorldItem)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to spawn NewWorldItem"));
        return false;
    }

    if (!Item->FindDataTableByItemType())
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to find DataTable by ItemType"));
        NewWorldItem->Destroy();
        return false;
    }

    if (bIsItemHeld && Item == HeldItem)
    {
        ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(HeldItem);
        if (TakeRemoveItemIF && TakeRemoveItemIF->CanDrop() && RemoveItemFromInventory(Item, false))
        {
            if (TakeRemoveItemIF->UseStaticMesh())
            {
                TakeRemoveItemIF->GetHeldMeshItem()->Destroy();
                GetOwnerCharacter()->AnimItemBlendTypeNow = EAnimItemBlendType::None;
            }
            else
            {
                // логика с скелет мешем 
            }

            HeldItem = nullptr;
            bIsItemHeld = false;

            HandleItemDrop(Item, NewWorldItem, TakeRemoveItemIF);
            return true;
        }
    }
    else if (RemoveItemFromInventory(Item, false))
    {
        HandleItemDrop(Item, NewWorldItem, nullptr);
        return true;
    }

    NewWorldItem->Destroy();
    return false;
}

void UInventoryComponent::HandleItemDrop(UItemBase* Item, AWorldItem* NewWorldItem, ITakeRemoveItemIF* TakeRemoveItemIF)
{
    if (!Item || !NewWorldItem) return;

    UItemDynamicInfo* ItemDynamic = DuplicateObject<UItemDynamicInfo>(Item->GetItemDynamicInfo(), GetOuter());
    if (!ItemDynamic)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to duplicate ItemDynamicInfo"));
        return;
    }

    ItemDynamic->QuantityItems = 1;
    NewWorldItem->LoadDataToWorldItem(ItemDynamic, Item->GetItemStaticInfo(), Item->GetClass());
    NewWorldItem->SetActorLocation(GetOwnerCharacter()->GetActorLocation() + GetOwnerCharacter()->GetActorForwardVector() * PlayerDropLocationOffset);
    NewWorldItem->SetActorScale3D(Item->GetItemStaticInfo()->WorldItemScale);

    if (TakeRemoveItemIF)
    {
        TakeRemoveItemIF->OnDropItem(NewWorldItem);
    }

    if (Item->GetItemDynamicInfo()->QuantityItems == 0)
    {
        Item->GetItemDynamicInfo()->ConditionalBeginDestroy();
        Item->ConditionalBeginDestroy();
    }
}

bool UInventoryComponent::TakeItemToHandsByID(int32 ID)
{
    if (bIsItemHeld || !Inventory[QuickInventoryContainerType].ContainerInventory[ID])
    {
        return false;
    }

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(Inventory[QuickInventoryContainerType].ContainerInventory[ID]);
    if (!TakeRemoveItemIF)
    {
        return false;
    }

    const FHeldItemInfo& HeldItemInfo = TakeRemoveItemIF->GetHeldItemInfo();
    if (HeldItemInfo.ItemSocketName == "None" || HeldItemInfo.ItemAnimBlendType == EAnimItemBlendType::None)
    {
        return false;
    }

    if (TakeRemoveItemIF->UseStaticMesh())
    {
        AStaticMeshActor* HeldMeshItem = GetWorld()->SpawnActor<AStaticMeshActor>();
        if (!HeldMeshItem)
        {
            return false;
        }

        TakeRemoveItemIF->SetHeldMeshItem(HeldMeshItem);
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
    else
    {
        // логика с скелет мешем 
    }

    return false;
}

bool UInventoryComponent::RemoveItemFromHands()
{
    if (!bIsItemHeld || !HeldItem) return false;

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(HeldItem);
    if (!TakeRemoveItemIF) return false;

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