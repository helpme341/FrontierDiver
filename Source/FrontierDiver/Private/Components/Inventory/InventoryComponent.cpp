// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/Inventory/Items/ItemsTypes/JewelryItem.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/Items/Interfaces/TakeRemoveItemIF.h"
#include "Components/Inventory/Items/Interfaces/InteractItemIF.h"
#include "Engine/StaticMeshActor.h"
#include "Components/Inventory/Items/ItemsTypes/BreathingTankItem.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

UInventoryComponent::UInventoryComponent()
{
    
}

/*
* return 0 =  ошибка добавления оно не возможно
* return 1 =  предмет был добавлен
* return 2 =  дредмет был застакан с оналогичным и удален
*/
int UInventoryComponent::AddItemToInventory(UItemBase* Item, UItemBase*& ItemResult)
{ 
    if (!Item || Item->ItemID != 99 || !Item->GetItemDynamicInfo() || !Item->FindDataTableByItemType(GetWorld()) ||
        !Item->GetItemStaticInfo() || !Inventory.Contains(Item->GetItemStaticInfo()->ItemContainerType) ||
        Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory.IsEmpty()) { return 0; }

    bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;

    if (Item->GetItemDynamicInfo()->ItemTypeName != "None" && Item->GetItemDynamicInfo()->QuantityItems != 0)
    {
        for (int32 Counter = 0; Counter < Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory.Num(); Counter++)
        {
            UItemBase* ItemOnInspection = Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory[Counter].Item.Get();

            if (bCheckQuantity && ItemOnInspection && ItemOnInspection->GetItemDynamicInfo()->ItemTypeName == Item->GetItemDynamicInfo()->ItemTypeName && ItemOnInspection->GetClass() == Item->GetClass())
            {
                if (ItemOnInspection->GetItemDynamicInfo()->QuantityItems + Item->GetItemDynamicInfo()->QuantityItems <= Item->GetItemStaticInfo()->MaxQuantityItemsInSlot)
                {
                    ItemOnInspection->GetItemDynamicInfo()->QuantityItems += Item->GetItemDynamicInfo()->QuantityItems;
                    Item->ConditionalBeginDestroy();
                    ItemResult = ItemOnInspection;
                    InventoryWidget->UpdateWidgetByItem(ItemOnInspection, false);
                    return 2;
                }
            }

            if (!ItemOnInspection)
            {
                Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory[Counter].Item.Reset(Item);
                Item->ItemID = Counter;
                InventoryWidget->UpdateWidgetByItem(Item, false);
                return 1;
            }
        }
    }
    else
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("ItemDynamicInfo is nullptr or invalid"));
    }
    return 0;
}

/*
* return 0 =  ошибка удаления оно не возможно
* return 1 =  предмет удален из инвеноря 
* return 2 =  предмет был застакон и из него был удален 1 копия
*/
int UInventoryComponent::RemoveItemFromInventory(UItemBase* Item)
{
    if (!Item || Item->ItemID == 99 || !Item->FindDataTableByItemType(GetWorld()) || !Item->GetItemDynamicInfo() ||
        !Item->GetItemStaticInfo() || !Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory[Item->ItemID].Item) { return 0; }

    if (Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1 && Item->GetItemDynamicInfo()->QuantityItems > 1)
    {
        Item->GetItemDynamicInfo()->QuantityItems--;
        InventoryWidget->UpdateWidgetByItem(Item, false);
        return 2;
    }
    else if (Inventory.Contains(Item->GetItemStaticInfo()->ItemContainerType))
    {
        Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory[Item->ItemID].Item = nullptr;
        InventoryWidget->UpdateWidgetByItem(Item, true);
        Item->ConditionalBeginDestroy();
        return 1;
    }
    return 0;
}

bool UInventoryComponent::PickupItemToInventory(AWorldItem* Item)
{
    if (!Item) { return false; }

    UItemBase* NewItem = NewObject<UItemBase>(this, Item->ItemType);

    if (!NewItem)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to create NewItem"));
        return false;
    }

    if (Item->ItemDynamicInfo)
    {
        if (NewItem->GetItemDynamicInfo()) { NewItem->GetItemDynamicInfo()->ConditionalBeginDestroy(); }
        NewItem->SetItemDynamicInfo(Item->ItemDynamicInfo.Get());
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

    UItemBase* ItemResult;
    switch (AddItemToInventory(NewItem, ItemResult))
    {
    case 0:
    {
        NewItem->ConditionalBeginDestroy();
        return false;
    }
    case 1:
    {
        NewItem->OnPickupItemToInventory(Item);
        Item->Destroy();
        return true;
    }
    case 2:
    {
        ItemResult->OnPickupItemToInventory(Item);
        Item->Destroy();
        return true;
    }
    default:
        return false;
    }
    return false;
}

/*
* return 0 =  ошибка
* return 1 =  предмет удален из инвеноря
* return 2 =  предмет был застакон и из него был удален
*/
int UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
    if (!Item || !Item->bIsPlayerCanDropThisItem || !Item->FindDataTableByItemType(GetWorld()))
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Item is nullptr or cannot be dropped"));
        return 0;
    }

    AWorldItem* NewWorldItem = Item->GetWorld()->SpawnActor<AWorldItem>();
    if (!NewWorldItem)
    {
        UE_LOG(LogInventoryComponent, Warning, TEXT("Failed to spawn NewWorldItem"));
        return 0;
    }

    UItemDynamicInfo* ItemDynamicInfo = Item->GetItemDynamicInfo();
    if (!ItemDynamicInfo)
    {
        NewWorldItem->Destroy();
        return 0;
    }

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(Inventory[Item->GetItemStaticInfo()->ItemContainerType].ContainerInventory[Item->ItemID].Item.Get());
    int Result = RemoveItemFromInventory(Item);
    if (Result != 0)
    {
        bool ItemHeld = bIsItemHeld && Item == HeldItem.Get();
        FVector DropLocation = GetOwnerCharacter()->GetActorLocation() + GetOwnerCharacter()->GetActorForwardVector() * PlayerDropLocationOffset;

        if (Result == 2)
        {
            ItemDynamicInfo = DuplicateObject<UItemDynamicInfo>(ItemDynamicInfo, GetOuter());
            ItemDynamicInfo->QuantityItems = 1;
        }

        NewWorldItem->LoadDataToWorldItem(ItemDynamicInfo, Item->GetItemStaticInfo(), Item->GetClass(), DropLocation);
        NewWorldItem->SetActorScale3D(Item->GetItemStaticInfo()->WorldItemScale);
        if (TakeRemoveItemIF) { TakeRemoveItemIF->OnDropItem(NewWorldItem); }

        if (ItemHeld)
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
        }
        return Result;
    }
    NewWorldItem->Destroy();
    return 0;
}

bool UInventoryComponent::HeldItemToHandsByID(int32 ID)
{
    if (bIsItemHeld || !Inventory[QuickInventoryContainerType].ContainerInventory[ID].Item)
    {
        return false;
    }

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(Inventory[QuickInventoryContainerType].ContainerInventory[ID].Item.Get());
    if (!TakeRemoveItemIF)
    {
        return false;
    }

    const FHeldItemInfo* HeldItemInfo = TakeRemoveItemIF->GetHeldItemInfo();
    if (HeldItemInfo->ItemSocketName == "None" || HeldItemInfo->ItemAnimBlendType == EAnimItemBlendType::None)
    {
        return false;
    }

    if (TakeRemoveItemIF->UseStaticMesh())
    {
        AStaticMeshActor* HeldMeshItem = GetWorld()->SpawnActor<AStaticMeshActor>();
        if (!HeldMeshItem) { return false; }

        TakeRemoveItemIF->SetHeldMeshItem(HeldMeshItem);
        HeldMeshItem->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
        HeldMeshItem->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        HeldMeshItem->AttachToComponent(GetOwnerCharacter()->Mesh1P, AttachmentRules, HeldItemInfo->ItemSocketName);
        HeldMeshItem->AddActorLocalTransform(HeldItemInfo->ItemItemAttachOffset);

        HeldItem.Reset(Inventory[QuickInventoryContainerType].ContainerInventory[ID].Item.Get());
        HeldMeshItem->SetActorScale3D(HeldItem->GetItemStaticInfo()->WorldItemScale);

        if (HeldMeshItem->GetStaticMeshComponent() && HeldItem->GetItemStaticInfo()->WorldItemStaticMesh)
        {
            HeldMeshItem->GetStaticMeshComponent()->SetStaticMesh(HeldItem->GetItemStaticInfo()->WorldItemStaticMesh);
        }

        TakeRemoveItemIF->OnTakeItem(this);
        GetOwnerCharacter()->AnimItemBlendTypeNow = HeldItemInfo->ItemAnimBlendType;
        bIsItemHeld = true;
        return true;
    }
    else
    {
        // логика с скелет мешем 
    }

    return false;
}

bool UInventoryComponent::HeldBreathingTankItemByID(int32 ID)
{
    if (!Inventory[EContainerType::BreathingTanks].ContainerInventory[ID].Item) { return false; }

    UBreathingTankItem* BreathingTankItem = Cast<UBreathingTankItem>(Inventory[EContainerType::BreathingTanks].ContainerInventory[ID].Item.Get());
    if (!BreathingTankItem) { return false; }
    if (!BreathingTankItem->FindDataTableByItemType(GetWorld())) { return false; }
    if (BreathingTankItem == HeldBreathingTankItem.Get()) { return false; }

    if (bIsBreathingTankItemHeld)
    {
        if (!HeldBreathingTankItem->FindDataTableByItemType(GetWorld())) { return false; }
        GetOwnerCharacter()->DecreaseMaxAir(HeldBreathingTankItem->GetBreathingTankMaxAir());
        GetOwnerCharacter()->UseAir(HeldBreathingTankItem->GetBreathingTankAir());
        HeldBreathingTankItem.Reset(BreathingTankItem);
        GetOwnerCharacter()->IncreaseMaxAir(HeldBreathingTankItem->GetBreathingTankMaxAir());
        GetOwnerCharacter()->ReplenishAir(HeldBreathingTankItem->GetBreathingTankAir());
        return true;
    }
    else
    {
        HeldBreathingTankItem.Reset(BreathingTankItem);
        GetOwnerCharacter()->CurrentMaxAir += HeldBreathingTankItem->GetBreathingTankMaxAir();
        GetOwnerCharacter()->CurrentAir += HeldBreathingTankItem->GetBreathingTankAir();
        bIsBreathingTankItemHeld = true;
        return true;
    }
    return false;
}

bool UInventoryComponent::RemoveBreathingTankItem()
{
    if (!bIsBreathingTankItemHeld && !HeldBreathingTankItem) { return false; }
    if(!HeldBreathingTankItem->FindDataTableByItemType(GetWorld())) { return false; }
    HeldBreathingTankItem->SetBreathingTankAir(GetOwnerCharacter()->CurrentAir - GetOwnerCharacter()->MaxAir);
    GetOwnerCharacter()->CurrentMaxAir -= HeldBreathingTankItem->GetBreathingTankMaxAir();
    GetOwnerCharacter()->CurrentAir -= HeldBreathingTankItem->GetBreathingTankAir();
    HeldBreathingTankItem = nullptr;
    bIsBreathingTankItemHeld = false;
    return true;
}

bool UInventoryComponent::RemoveItemFromHands()
{
    if (!bIsItemHeld || !HeldItem) return false;

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(HeldItem.Get());
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
        IInteractItemIF* InteractItemIF = Cast<IInteractItemIF>(HeldItem.Get());
        if (InteractItemIF) { InteractItemIF->FirstInteract(this); }
        return true;
    }
    return false;
}

bool UInventoryComponent::SecondInteractWithHeldItem()
{
    if (bIsItemHeld)
    {
        IInteractItemIF* InteractItemIF = Cast<IInteractItemIF>(HeldItem.Get());
        if (InteractItemIF) { InteractItemIF->SecondInteract(this); }
        return true;
    }
    return false;
}

bool UInventoryComponent::ThirdInteractWithHeldItem()
{
    if (bIsItemHeld)
    {
        IInteractItemIF* InteractItemIF = Cast<IInteractItemIF>(HeldItem.Get());
        if (InteractItemIF) { InteractItemIF->ThirdInteract(this); }
        return true;
    }
    return false;
}

void UInventoryComponent::OnInventoryItemWidgetConstructed()
{
    if (MaxCounterOfCreatedWidgets != 0)
    {
        CounterOfCreatedWidgets++;
        if (CounterOfCreatedWidgets == MaxCounterOfCreatedWidgets)
        {
            InventoryWidget->UpdateWidgetsUsability();
            InventoryWidget->UpdateAllWidgets();
            InventoryWidget->SetVisibility(ESlateVisibility::Visible);
            InventoryWidget->SetNotQuickInventoryVisibility(true);
        }
    }
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    if (InventoryWidgetClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            //for (auto& Elem : Inventory) { MaxCounterOfCreatedWidgets += Elem.Value.ContainerInventory.Num(); }
            InventoryWidget.Reset(CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass));
            if (InventoryWidget)
            {
                InventoryWidget->InventoryComponent.Reset(this);
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