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

bool UInventoryComponent::AddItemToInventory(UItemBase* Item)
{
    UItemBase* ItemResult;
    return AddItemToInventory(Item, ItemResult) != 0;
}

/*
* return 0 = ошибка добавления, оно не возможно
* return 1 = предмет был добавлен
* return 2 = предмет был застакан с аналогичным и удален
*/
int UInventoryComponent::AddItemToInventory(UItemBase* Item, UItemBase*& ItemResult)
{
    // Проверка валидности предмета
    if (!Item || Item->ItemSlotInfo->ID != 99 || !Item->GetItemDynamicInfo() ||
        !Item->FindDataTableForItem(GetWorld()) || !Item->GetItemStaticInfo()) { return 0; }

    bool bCheckQuantity = Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1;

    // Итерация по типам контейнеров, в которые может быть добавлен предмет
    for (EContainerType ContainerType : Item->GetItemStaticInfo()->ItemContainerTypes)
    {
        if (Inventory.Contains(ContainerType))
        {
            auto& ContainerInventory = Inventory[ContainerType].ContainerInventory;

            // Итерация по элементам инвентаря
            for (int32 Index = 0; Index < ContainerInventory.Num(); ++Index)
            {
                FSharedContainerBase& ExistingItem = ContainerInventory[Index];
                UItemBase* ItemOnInspection = ExistingItem.Item.Get();

                if (ItemOnInspection)
                {
                    // Проверка и добавление предмета при достаточном количестве места
                    if (bCheckQuantity &&
                        ItemOnInspection->GetItemDynamicInfo()->ItemTypeName == Item->GetItemDynamicInfo()->ItemTypeName &&
                        ItemOnInspection->GetClass() == Item->GetClass())
                    {
                        if (ItemOnInspection->GetItemDynamicInfo()->QuantityItems + Item->GetItemDynamicInfo()->QuantityItems <= Item->GetItemStaticInfo()->MaxQuantityItemsInSlot)
                        {
                            ItemOnInspection->GetItemDynamicInfo()->QuantityItems += Item->GetItemDynamicInfo()->QuantityItems;
                            Item->ConditionalBeginDestroy();
                            ItemResult = ItemOnInspection;
                            InventoryWidget->UpdateWidgetByItem(ItemOnInspection, false);
                            ItemOnInspection->OnAddItemToInventory();
                            return 2;
                        }
                    }
                }
                else
                {
                    // Помещение предмета в свободный слот
                    ContainerInventory[Index].Item.Reset(Item);
                    ItemResult = Item;
                    Item->ItemSlotInfo = MakeShared<FSlotInfo>(ContainerType, Index);
                    InventoryWidget->UpdateWidgetByItem(Item, false);
                    Item->OnAddItemToInventory();
                    return 1; 
                }
            }
        }
    }

    // Если нет подходящих слотов
    return 0;
}

/*
* return 0 =  ошибка удаления оно не возможно
* return 1 =  предмет удален из инвеноря 
* return 2 =  предмет был застакон и из него был удален 1 копия
*/
int UInventoryComponent::RemoveItemFromInventory(UItemBase* Item, bool RemoveItem)
{
    if (!Item || Item->ItemSlotInfo->ID == 99 || !Item->FindDataTableForItem(GetWorld()) || !Item->GetItemDynamicInfo() || !Item->GetItemStaticInfo()) { return 0; }

    if (Item->GetItemStaticInfo()->MaxQuantityItemsInSlot > 1 && Item->GetItemDynamicInfo()->QuantityItems > 1)
    {
        Item->GetItemDynamicInfo()->QuantityItems--;
        InventoryWidget->UpdateWidgetByItem(Item, false);
        return 2;
    }
    else if (Inventory.Contains(Item->ItemSlotInfo->ContainerType))
    {
        Inventory[Item->ItemSlotInfo->ContainerType].ContainerInventory[Item->ItemSlotInfo->ID].Item.Reset();
        InventoryWidget->UpdateWidgetByItem(Item, true);
        if (RemoveItem) { Item->ConditionalBeginDestroy(); }
        else { Item->ItemSlotInfo->ID = 99; }
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
    int32 Result = AddItemToInventory(NewItem, ItemResult);

    if (Result != 0)
    {
        NewItem->OnPickupItemToInventory(Item);
        Item->Destroy();
        return true;
    }
    NewItem->ConditionalBeginDestroy();
    return false;
}

/*
* return 0 =  ошибка
* return 1 =  предмет удален из инвеноря
* return 2 =  предмет был застакон и из него был удален
*/
int UInventoryComponent::DropItemFromInventory(UItemBase* Item)
{
    if (!Item || !Item->bIsPlayerCanDropThisItem || !Item->FindDataTableForItem(GetWorld()))
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

    ITakeRemoveItemIF* TakeRemoveItemIF = Cast<ITakeRemoveItemIF>(Inventory[Item->ItemSlotInfo->ContainerType].ContainerInventory[Item->ItemSlotInfo->ID].Item.Get());
    int Result = RemoveItemFromInventory(Item, true);
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

            HeldItem.Reset();
            bIsItemHeld = false;
        }
        return Result;
    }
    NewWorldItem->Destroy();
    return 0;
}

bool UInventoryComponent::MoveItemFromStartSlotToTargetSlot(FSlotInfo& StartSlot, FSlotInfo& TargetSlot)
{
    if (!StartSlot.IsValid() || !Inventory[StartSlot.ContainerType].ContainerInventory[StartSlot.ID].Item.IsValid()) { return false; }
    if (StartSlot == TargetSlot) { return false; }

    UItemBase* ItemInStartSlot = Inventory[StartSlot.ContainerType].ContainerInventory[StartSlot.ID].Item.Get();
    if (!ItemInStartSlot->FindDataTableForItem(GetWorld())) { return false; }
    if (!CheckContainerTypesForType(ItemInStartSlot->GetItemStaticInfo()->ItemContainerTypes, TargetSlot.ContainerType)) { return false; }

    UItemBase* ItemInTargetSlot = Inventory[TargetSlot.ContainerType].ContainerInventory[TargetSlot.ID].Item.Get();


    if (ItemInTargetSlot)
    {
        if (ItemInStartSlot->FindDataTableForItem(GetWorld()))
        {
            if (ItemInStartSlot->GetItemDynamicInfo()->ItemTypeName == ItemInTargetSlot->GetItemDynamicInfo()->ItemTypeName && ItemInStartSlot->GetClass() == ItemInTargetSlot->GetClass())
            {
                if (ItemInStartSlot->GetItemDynamicInfo()->QuantityItems + ItemInTargetSlot->GetItemDynamicInfo()->QuantityItems > ItemInStartSlot->GetItemStaticInfo()->MaxQuantityItemsInSlot)
                {
                    // перегрузка предмета половину в начало половину в таргет 

                    ItemInStartSlot->GetItemDynamicInfo()->QuantityItems = ItemInStartSlot->GetItemDynamicInfo()->QuantityItems + ItemInTargetSlot->GetItemDynamicInfo()->QuantityItems - ItemInTargetSlot->GetItemStaticInfo()->MaxQuantityItemsInSlot;
                    InventoryWidget->UpdateWidgetByItem(ItemInStartSlot, false);
                   
                    ItemInTargetSlot->GetItemDynamicInfo()->QuantityItems = ItemInTargetSlot->GetItemStaticInfo()->MaxQuantityItemsInSlot;
                    InventoryWidget->UpdateWidgetByItem(ItemInTargetSlot, false);
                    return true;
                }
                else
                {
                    // перегрузка стак придмет в таргет 
                    InventoryWidget->UpdateWidgetByItem(ItemInStartSlot, true);
                    ItemInTargetSlot->GetItemDynamicInfo()->QuantityItems += ItemInStartSlot->GetItemDynamicInfo()->QuantityItems;
                    InventoryWidget->UpdateWidgetByItem(ItemInTargetSlot, false);
                    Inventory[StartSlot.ContainerType].ContainerInventory[StartSlot.ID].Item.Reset();
                    ItemInStartSlot->ConditionalBeginDestroy();
                    return true;
                }
            }
           

            // меняем приредметы местами 
            ItemInStartSlot->ItemSlotInfo = MakeShared<FSlotInfo>(TargetSlot);
            Inventory[TargetSlot.ContainerType].ContainerInventory[TargetSlot.ID].Item.Reset(ItemInStartSlot);
            InventoryWidget->UpdateWidgetByItem(ItemInStartSlot, false);

            ItemInTargetSlot->ItemSlotInfo = MakeShared<FSlotInfo>(StartSlot);
            Inventory[StartSlot.ContainerType].ContainerInventory[StartSlot.ID].Item.Reset(ItemInTargetSlot);
            InventoryWidget->UpdateWidgetByItem(ItemInTargetSlot, false);
            return true;
        }
    }
    else
    {
        // созраняет предмет в пустой таргет
        Inventory[StartSlot.ContainerType].ContainerInventory[StartSlot.ID].Item.Reset();
        Inventory[TargetSlot.ContainerType].ContainerInventory[TargetSlot.ID].Item.Reset(ItemInStartSlot);
        InventoryWidget->UpdateWidgetByItem(ItemInStartSlot, true);
        ItemInStartSlot->ItemSlotInfo = MakeShared<FSlotInfo>(TargetSlot);
        InventoryWidget->UpdateWidgetByItem(ItemInStartSlot, false);
        return true;
    }
    return false;
}

bool UInventoryComponent::CheckContainerTypesForType(const TSet<EContainerType>& ItemContainerTypes, EContainerType TargetContainerType)
{
    for (EContainerType ContainerType : ItemContainerTypes) { if (ContainerType == TargetContainerType) { return true; } }
    return false;
}

void UInventoryComponent::OpenCloseInventory()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        if (InventoryWidget->InventoryIsHidden())
        {
            // Открытие инвентаря
            PlayerController->bShowMouseCursor = true;
            PlayerController->bEnableClickEvents = true;
            PlayerController->bEnableMouseOverEvents = true;
            PlayerController->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(InventoryWidget->TakeWidget()).SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock));
            InventoryWidget->OnOpenInventory();
            InventoryWidget->SetNotQuickInventoryVisibility(false);
        }
        else
        {
            // Закрытие инвентаря
            PlayerController->bShowMouseCursor = false;
            PlayerController->bEnableClickEvents = false;
            PlayerController->bEnableMouseOverEvents = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
            InventoryWidget->OnCloseInventory();
            InventoryWidget->SetNotQuickInventoryVisibility(true);
        }
    }
}

bool UInventoryComponent::HeldItemToHandsByID(int32 ID)
{
    if (bIsItemHeld || !Inventory[QuickInventoryContainerType].ContainerInventory[ID].Item)
    {
        return RemoveItemFromHands();
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
    if (!Inventory[EContainerType::BreathingTanks].ContainerInventory.IsValidIndex(ID) ||
        !Inventory[EContainerType::BreathingTanks].ContainerInventory[ID].Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeldBreathingTankItemByID: Invalid index or item is null. ID: %d"), ID);
        return false;
    }

    UBreathingTankItem* BreathingTankItem = Cast<UBreathingTankItem>(Inventory[EContainerType::BreathingTanks].ContainerInventory[ID].Item.Get());
    if (!BreathingTankItem || !BreathingTankItem->FindDataTableForItem(GetWorld()))
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
        if (!HeldBreathingTankItem->FindDataTableForItem(GetWorld()))
        {
            UE_LOG(LogTemp, Warning, TEXT("HeldBreathingTankItemByID: Current held item DataTable not found."));
            return false;
        }

        // cдесь логика обновления балона визуално если нужно))
    }

    // Назначаем новый баллон
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

    if (!HeldBreathingTankItem->FindDataTableForItem(GetWorld()))
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
        HeldItem.Reset();
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