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

void UInventoryWidget::ShowItemInfo(UItemBase* Item)///////////////////////////////
{
    if (Item && Item->GetItemDynamicInfo() && !bIsInventoryHidden && !bIsShowingItemInfo)
    {
        if (Item->FindDataTableByItemType(GetWorld()) || Item->GetItemStaticInfo())
        {
            ItemImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
            ItemDescription->SetText(Item->GetItemStaticInfo()->ItemDescription);
            ItemNameTextBlock->SetText(FText::FromName(Item->GetItemDynamicInfo()->ItemTypeName));

            FString QuantityString = FString::Printf(TEXT("%d/%d"), Item->GetItemDynamicInfo()->QuantityItems, Item->GetItemStaticInfo()->MaxQuantityItemsInSlot);
            ItemQuantityTextBlock->SetText(FText::FromString(QuantityString));

            ItemImage->SetVisibility(ESlateVisibility::Visible);
            ItemNameTextBlock->SetVisibility(ESlateVisibility::Visible);
            ItemQuantityTextBlock->SetVisibility(ESlateVisibility::Visible);
            ItemDescription->SetVisibility(ESlateVisibility::Visible);
            bIsShowingItemInfo = true;
            return;
        }
        return;
    }
    else if (!bIsInventoryHidden && bIsShowingItemInfo)
    {
        if (DefaultItemWidgetTexture)
        {
            ItemImage->SetBrushFromTexture(DefaultItemWidgetTexture);
            ItemImage->SetVisibility(ESlateVisibility::Hidden);
        }

        ItemNameTextBlock->SetText(FText::GetEmpty());
        ItemNameTextBlock->SetVisibility(ESlateVisibility::Hidden);

        ItemQuantityTextBlock->SetText(FText::GetEmpty());
        ItemQuantityTextBlock->SetVisibility(ESlateVisibility::Hidden);

        ItemDescription->SetText(FText::GetEmpty());
        ItemDescription->SetVisibility(ESlateVisibility::Hidden);
        bIsShowingItemInfo = false;
        return;
    }
    bIsShowingItemInfo = false;
}

void UInventoryWidget::DropItemFromWidget(UItemBase* Item)////////////////
{
    if (Item && !bIsInventoryHidden)
    {
        int Result = InventoryComponent->DropItemFromInventory(Item);
        if (Result != 0)
        {
            if (bIsShowingItemInfo)
            {
                if (Result == 1)
                {
                    ShowItemInfo(nullptr);
                }
                else if (Result == 2)
                {
                    ShowItemInfo(Item);
                }
            }
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
                NewWidget->InventoryWidget.Reset(this);
                NewWidget->Item.Reset(InventoryComponent->Inventory[Elem.Key].ContainerInventory[Counter].Item.Get());

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
                Widgets[Elem.Key].Array[Counter].ItemWidget.Reset(NewWidget);
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
            UpdateWidget(Elem.Value.ContainerInventory[Counter].Item.Get(), Widgets[Elem.Key].Array[Counter].ItemWidget.Get());
        }
    }
}

void UInventoryWidget::UpdateWidgetByItem(UItemBase* Item, bool Clear)//////////////
{
    if (Clear)
    {
        UpdateWidget(nullptr, Widgets[Item->GetItemStaticInfo()->ItemContainerType].Array[Item->ThisItemID].ItemWidget.Get());
    }
    else
    {
        if (Item)
        {
            UpdateWidget(Item, Widgets[Item->GetItemStaticInfo()->ItemContainerType].Array[Item->ThisItemID].ItemWidget.Get());
        }
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
            ItemWidget->WidgetImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
            ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), Item->GetItemDynamicInfo()->QuantityItems)));
            ItemWidget->Item.Reset(Item);
            ItemWidget->WidgetTextBlock->SetText(FText::AsNumber(Item->GetItemDynamicInfo()->QuantityItems));
            ItemWidget->bIsThisEmptyWidget = false;
        }
        else if (!ItemWidget->bIsThisEmptyWidget)
        {
            ItemWidget->WidgetImage->SetBrushFromTexture(DefaultItemWidgetTexture);
            ItemWidget->WidgetTextBlock->SetText(FText());
            ItemWidget->Item = nullptr;
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
                if (Widgets[ContainerSettings.Key].Array[Counter].ItemWidget)
                {
                    Widgets[ContainerSettings.Key].Array[Counter].ItemWidget->SetVisibility(NewWidgetsState);
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
            if (Widgets[ContainerSettings.Key].Array[Counter].ItemWidget)
            {
                Widgets[ContainerSettings.Key].Array[Counter].ItemWidget->SetVisibility(NewWidgetsState);
            }
        }
    }
    bIsAllInventoryHidden = Hide;
}
