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
#include "Widgets/MainWidget.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

UInventoryComponent::UInventoryComponent()
{
    
}

/*
* return 0 =  ������ ���������� ��� �� ��������
* return 1 =  ������� ��� ��������
* return 2 =  ������� ��� �������� � ����������� � ������
*/
int UInventoryComponent::AddItemToInventory(UItemBase* Item, UItemBase*& ItemResult)
{ 
    if (!Item || Item->ItemID != 99 || !Item->GetItemDynamicInfo() || !Item->FindDataTableByItemType(GetWorld()) || !Item->GetItemStaticInfo() ) { return 0; }

    EContainerType ItemContainerType;
    for (EContainerType Enum : Item->GetItemStaticInfo()->ItemContainerTypes)
    {
        if (Inventory.Contains(Enum) && !Inventory[Enum].ContainerInventory.IsEmpty()) { ItemContainerType = Enum; break; }
    }
    if (ItemContainerType == EContainerType::None) { return 0; }

    bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;

    if (Item->GetItemDynamicInfo()->ItemTypeName != "None" && Item->GetItemDynamicInfo()->QuantityItems != 0)
    {
        for (int32 Counter = 0; Counter < Inventory[ItemContainerType].ContainerInventory.Num(); Counter++)
        {
            UItemBase* ItemOnInspection = Inventory[ItemContainerType].ContainerInventory[Counter].Item.Get();

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
                Inventory[ItemContainerType].ContainerInventory[Counter].Item.Reset(Item);
                Item->ItemID = Counter;
                Item->ItemContainerType = ItemContainerType;
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
* return 0 =  ������ �������� ��� �� ��������
* return 1 =  ������� ������ �� �������� 
* return 2 =  ������� ��� �������� � �� ���� ��� ������ 1 �����
*/
int UInventoryComponent::RemoveItemFromInventory(UItemBase* Item)
{
    if (!Item || Item->ItemID == 99 || !Item->FindDataTableByItemType(GetWorld()) || !Item->GetItemDynamicInfo() || !Item->GetItemStaticInfo()) { return 0; }

    if (Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1 && Item->GetItemDynamicInfo()->QuantityItems > 1)
    {
        Item->GetItemDynamicInfo()->QuantityItems--;
        InventoryWidget->UpdateWidgetByItem(Item, false);
        return 2;
    }
    else if (Inventory.Contains(Item->ItemContainerType))
    {
        Inventory[Item->ItemContainerType].ContainerInventory[Item->ItemID].Item = nullptr;
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
* return 0 =  ������
* return 1 =  ������� ������ �� ��������
* return 2 =  ������� ��� �������� � �� ���� ��� ������
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

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(Inventory[Item->ItemContainerType].ContainerInventory[Item->ItemID].Item.Get());
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
                // ������ � ������ ����� 
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
        // ������ � ������ ����� 
    }

    return false;
}

bool UInventoryComponent::HeldBreathingTankItemByID(int32 ID)
{
    if (!Inventory[EContainerType::BreathingTanks].ContainerInventory.IsValidIndex(ID) ||
        !Inventory[EContainerType::BreathingTanks].ContainerInventory[ID].Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeldBreathingTankItemByID: Invalid index or item is null. ID: %d"), ID);
        return false;
    }

    UBreathingTankItem* BreathingTankItem = Cast<UBreathingTankItem>(Inventory[EContainerType::BreathingTanks].ContainerInventory[ID].Item.Get());
    if (!BreathingTankItem || !BreathingTankItem->FindDataTableByItemType(GetWorld()))
    {
        UE_LOG(LogTemp, Warning, TEXT("HeldBreathingTankItemByID: Invalid BreathingTankItem or DataTable not found. ID: %d"), ID);
        return false;
    }

    if (BreathingTankItem == HeldBreathingTankItem.Get())
    {
        UE_LOG(LogTemp, Warning, TEXT("HeldBreathingTankItemByID: BreathingTankItem is already held. ID: %d"), ID);
        return false;
    }

    if (bIsBreathingTankItemHeld)
    {
        if (!HeldBreathingTankItem->FindDataTableByItemType(GetWorld()))
        {
            UE_LOG(LogTemp, Warning, TEXT("HeldBreathingTankItemByID: Current held item DataTable not found."));
            return false;
        }

        // c���� ������ ���������� ������ �������� ���� �����))
    }

    // ��������� ����� ������
    HeldBreathingTankItem.Reset(BreathingTankItem);
    bIsBreathingTankItemHeld = true;
    DiverCharacter->MainWidget->UpdateWidgetForAir();
    UE_LOG(LogTemp, Warning, TEXT("Breathing tank item held successfully. ID: %d"), ID);
    return true;
}

bool UInventoryComponent::RemoveBreathingTankItem()
{
    if (!bIsBreathingTankItemHeld || !HeldBreathingTankItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveBreathingTankItem: No item is currently held."));
        return false;
    }

    if (!HeldBreathingTankItem->FindDataTableByItemType(GetWorld()))
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveBreathingTankItem: Held item DataTable not found."));
        return false;
    }

    HeldBreathingTankItem.Reset();
    bIsBreathingTankItemHeld = false;
    DiverCharacter->MainWidget->UpdateWidgetForAir();

    UE_LOG(LogTemp, Warning, TEXT("Breathing tank item removed successfully."));
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
        // ������ � ������ ����� 
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
}

void UInventoryComponent::BeginLogic(AFrontierDiverCharacter* Character)
{
    DiverCharacter = Character;
    if (InventoryWidgetClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
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
