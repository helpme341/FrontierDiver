// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Inventory/Items/Modules/PickupDropItem/PickupDropItemIF.h"

DEFINE_LOG_CATEGORY(LogInventoryWidget);

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}


void UInventoryWidget::LoadWidgestSlots()
{
    for (auto& Elem : InventoryComponent->Inventory)
    {
        Widgets.Add(Elem.Key);
        Widgets[Elem.Key].Array.SetNum(InventoryComponent->Inventory[Elem.Key].ContainerInventory.Num());
    }
}

void UInventoryWidget::ShowItemInfo(UItemBase* Item)
{
    if (Item && !bIsInventoryHidden)
    {
        FItemDynamicInfoBase* ItemDynamicInfo;
        if (Item->IsValidLowLevel()) { ItemDynamicInfo = Item->GetItemDynamicInfo(); }
        else { bIsShowingItemInfo = false; return; }
        if (ItemDynamicInfo)
        {
            if (ItemDynamicInfo->QuantityItems == 0) { bIsShowingItemInfo = false; return; }
            FItemTableRowInfoBase* ItemStaticInfo = Item->GetItemStaticInfo();
            if (ItemStaticInfo)
            {
                if (ItemImage) { ItemImage->SetBrushFromTexture(ItemStaticInfo->ItemWidgetTexture); }

                if (ItemDescription) { ItemDescription->SetText(ItemStaticInfo->ItemDescription); }
            }
            else
            {
                UE_LOG(LogInventoryWidget, Warning, TEXT("ItemStaticInfo is nullptr"));
                bIsShowingItemInfo = false;
                return;
            }


            if (ItemDynamicInfo)
            {
                if (ItemNameTextBlock) { ItemNameTextBlock->SetText(FText::FromName(ItemDynamicInfo->ItemTypeName)); }
            }
            else
            {
                UE_LOG(LogInventoryWidget, Warning, TEXT("ItemDynamicInfo is nullptr"));
                bIsShowingItemInfo = false;
                return;
            }

            FString QuantityString = FString::Printf(TEXT("%d/%d"), ItemDynamicInfo->QuantityItems, ItemStaticInfo->MaxQuantityItemsInSlot);
            if (ItemQuantityTextBlock) { ItemQuantityTextBlock->SetText(FText::FromString(QuantityString)); }


            if (ItemImage) { ItemImage->SetVisibility(ESlateVisibility::Visible); }
            if (ItemNameTextBlock) { ItemNameTextBlock->SetVisibility(ESlateVisibility::Visible); }
            if (ItemQuantityTextBlock) { ItemQuantityTextBlock->SetVisibility(ESlateVisibility::Visible); }
            if (ItemDescription) { ItemDescription->SetVisibility(ESlateVisibility::Visible); }
            bIsShowingItemInfo = true;
            return;
        }
        bIsShowingItemInfo = false;
    }
    else if (!bIsInventoryHidden && bIsShowingItemInfo)
    {
        if (ItemImage)
        {
            ItemImage->SetBrushFromTexture(DefaultItemWidgetTexture);
            ItemImage->SetVisibility(ESlateVisibility::Hidden);
        }
        if (ItemNameTextBlock)
        {
            ItemNameTextBlock->SetText(FText::GetEmpty());
            ItemNameTextBlock->SetVisibility(ESlateVisibility::Hidden);
        }
        if (ItemQuantityTextBlock)
        {
            ItemQuantityTextBlock->SetText(FText::GetEmpty());
            ItemQuantityTextBlock->SetVisibility(ESlateVisibility::Hidden);
        }
        if (ItemDescription)
        {
            ItemDescription->SetText(FText::GetEmpty());
            ItemDescription->SetVisibility(ESlateVisibility::Hidden);
        }
        bIsShowingItemInfo = false;
    }
    bIsShowingItemInfo = false;
}

void UInventoryWidget::DropItemFromWidget(UItemBase* Item)
{
    if (Item && Item->IsValidLowLevel() && !bIsInventoryHidden)
    {
        if (IPickupDropItemIF* ItemIF = Cast<IPickupDropItemIF>(Item))
        {
            if (ItemIF->DropItem(InventoryComponent))
            {
                if (bIsShowingItemInfo)
                { 
                    if (Item->GetItemDynamicInfo()->QuantityItems == 0)
                    {
                        ShowItemInfo(nullptr);
                    }
                    else
                    {
                        ShowItemInfo(Item);
                    }
                }
            }
        }
        else
        {
            UE_LOG(LogInventoryWidget, Warning, TEXT("Item does not implement IPickupDropItemIF interface"));
        }
    }
}

void UInventoryWidget::CreateWidgets()
{
    for (auto& Elem : Widgets)
    {
        if (WidgetContainersSettings.Contains(Elem.Key))
        {
            const FWidgetContainerSettings& Settings = WidgetContainersSettings[Elem.Key];
            FVector2D CurrentPosition = Settings.ContainerStartPosition;
            int32 WidgetsInCurrentLine = 0;

            for (int32 Counter = 0; Counter < Elem.Value.Array.Num(); Counter++)
            {
                UInventoryItemWidget* NewWidget = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);

                if (!NewWidget)
                {
                    UE_LOG(LogInventoryWidget, Warning, TEXT("Failed to create inventory item widget"));
                    continue;
                }

                NewWidget->WidgetCanvasPanel->RenderTransform.Scale = Settings.WidgetSize;
                NewWidget->InventoryWidget = this;
                NewWidget->ItemContainerType = Elem.Key;
                NewWidget->ItemID = Counter;

                UCanvasPanelSlot* CanvasSlot = ParentCanvasPanel->AddChildToCanvas(NewWidget);
                CanvasSlot->SetPosition(CurrentPosition);

                CanvasSlot->SetAnchors(FAnchors(Settings.MinimumAnchors, Settings.MaximumAnchors));
                if (!CanvasSlot)
                {
                    UE_LOG(LogInventoryWidget, Warning, TEXT("Failed to add widget to canvas panel"));
                    continue;
                }


                WidgetsInCurrentLine++;
                if (WidgetsInCurrentLine >= Settings.MaxWidgetsInOneLine)
                {
                    CurrentPosition.Y += Settings.ContainerSkipBetweenWidgets.Y;
                    CurrentPosition.X = Settings.ContainerStartPosition.X;
                    WidgetsInCurrentLine = 0;
                }
                else
                {
                    CurrentPosition.X += Settings.ContainerSkipBetweenWidgets.X;
                }

                if (!Settings.bIsQuickInventory) { NewWidget->SetVisibility(ESlateVisibility::Hidden); }
                Widgets[Elem.Key].Array[Counter] = NewWidget;
            }
        }
    }
}

void UInventoryWidget::UpdateAllWidgets()
{
    for (auto& Elem : InventoryComponent->Inventory)
    {
        for (int32 Counter = 0; Counter < Elem.Value.ContainerInventory.Num(); Counter++)
        {
            UpdateWidget(Elem.Value.ContainerInventory[Counter], Widgets[Elem.Key].Array[Counter]);
        }
    }
}

void UInventoryWidget::UpdateWidgetByItem(UItemBase* Item, bool Clear)
{
    if (!Item)
    {
        UE_LOG(LogInventoryWidget, Warning, TEXT("Item is nullptr"));
        return;
    }

    if (Clear)
    {
        UpdateWidget(nullptr, Widgets[Item->GetItemStaticInfo()->ItemContainerType].Array[Item->ThisItemID]);
    }
    else
    {
        UpdateWidget(Item, Widgets[Item->GetItemStaticInfo()->ItemContainerType].Array[Item->ThisItemID]);
    }
}

void UInventoryWidget::UpdateWidget(UItemBase* Item, UInventoryItemWidget* ItemWidget)
{
    if (!ItemWidget)
    {
        UE_LOG(LogInventoryWidget, Warning, TEXT("ItemWidget is nullptr"));
        return;
    }

    if (ItemWidget)
    {
        if (Item)
        {
            if (Item->bUseCustomUpdateWidgetForThisItem) { Item->UpdateItemWidget(ItemWidget); }
            else
            {
                ItemWidget->WidgetImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
                ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), Item->GetItemDynamicInfo()->QuantityItems)));
                ItemWidget->ItemContainerType = Item->GetItemStaticInfo()->ItemContainerType;
                ItemWidget->ItemID = Item->ThisItemID;

                ItemWidget->WidgetTextBlock->SetText(FText::AsNumber(Item->GetItemDynamicInfo()->QuantityItems));
                ItemWidget->bIsThisEmptyWidget = false;
            }
        }
        else if (!ItemWidget->bIsThisEmptyWidget)
        {
            ItemWidget->WidgetImage->SetBrushFromTexture(DefaultItemWidgetTexture);
            ItemWidget->WidgetTextBlock->SetText(FText());
            ItemWidget->ItemID = -1;
            ItemWidget->bIsThisEmptyWidget = true;
        }
    }
}

void UInventoryWidget::SetNotQuickInventoryVisibility(bool Hide)
{
    if (bIsInventoryHidden && Hide || !bIsInventoryHidden && !Hide) { return; }
    ESlateVisibility NewWidgetsState = Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    for (auto& ContainerSettings : WidgetContainersSettings)
    {
        if (!ContainerSettings.Value.bIsQuickInventory)
        {
            for (int32 Counter = 0; Counter < Widgets[ContainerSettings.Key].Array.Num(); Counter++)
            {
                if (Widgets[ContainerSettings.Key].Array[Counter])
                {
                    Widgets[ContainerSettings.Key].Array[Counter]->SetVisibility(NewWidgetsState);
                }
            }
        }
    }
    if (Hide && bIsShowingItemInfo) { ShowItemInfo(nullptr); }
    bIsInventoryHidden = Hide;
}

void UInventoryWidget::SetAllInventoryVisibility(bool Hide)
{
    if (bIsAllInventoryHidden && Hide || !bIsAllInventoryHidden && !Hide) { return; }
    ESlateVisibility NewWidgetsState = Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    for (auto& ContainerSettings : WidgetContainersSettings)
    {
        for (int32 Counter = 0; Counter < Widgets[ContainerSettings.Key].Array.Num(); Counter++)
        {
            if (Widgets[ContainerSettings.Key].Array[Counter])
            {
                Widgets[ContainerSettings.Key].Array[Counter]->SetVisibility(NewWidgetsState);
            }
        }
    }
    bIsAllInventoryHidden = Hide;
}
