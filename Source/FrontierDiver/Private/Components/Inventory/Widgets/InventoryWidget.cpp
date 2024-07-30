// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/ItemBase.h"

DEFINE_LOG_CATEGORY(LogInventoryWidget);

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UInventoryWidget::ShowItemInfo(UItemBase* Item)
{
    if (Item && Item->GetItemDynamicInfo() && !bIsInventoryHidden && !ShowingItem.IsValid())
    {
        if (Item->bUseCustomShowItemInfo)
        {
            Item->CustomShowItemInfo();
            ItemImage->SetVisibility(ESlateVisibility::Visible);
            ItemNameTextBlock->SetVisibility(ESlateVisibility::Visible);
            ItemQuantityTextBlock->SetVisibility(ESlateVisibility::Visible);
            ItemDescription->SetVisibility(ESlateVisibility::Visible);
            bIsShowingItemInfo = true;
            ShowingItem.Reset(Item);
        }
        else
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
                ShowingItem.Reset(Item);
            }
        }
    }
    else if (!bIsInventoryHidden && bIsShowingItemInfo && ShowingItem.IsValid())
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
        ShowingItem.Reset();
    }
}

void UInventoryWidget::UpdateItemInfo()
{
    if (bIsShowingItemInfo && ShowingItem.IsValid())
    {
        if (ShowingItem->bUseCustomUpdateItemInfo) { ShowingItem->CustomUpdateItemInfo(); }
        else
        {
            ItemImage->SetBrushFromTexture(ShowingItem->GetItemStaticInfo()->ItemWidgetTexture);
            ItemDescription->SetText(ShowingItem->GetItemStaticInfo()->ItemDescription);
            ItemNameTextBlock->SetText(FText::FromName(ShowingItem->GetItemDynamicInfo()->ItemTypeName));
            FString QuantityString = FString::Printf(TEXT("%d/%d"), ShowingItem->GetItemDynamicInfo()->QuantityItems, ShowingItem->GetItemStaticInfo()->MaxQuantityItemsInSlot);
        }
    }
}

void UInventoryWidget::DropItemFromWidget(UItemBase* Item)
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

void UInventoryWidget::UpdateAllWidgets()
{
    for (auto& Elem : InventoryComponent->Inventory)
    {
        for (int32 Counter = 0; Counter < Elem.Value.ContainerInventory.Num(); Counter++)
        {
            OnUpdateWidgetsInfo.Broadcast(Elem.Value.ContainerInventory[Counter].Item.Get(), false);
        }
    }
}

void UInventoryWidget::UpdateWidgetsUsability()
{
    OnUpdateWidgetsUsability.Broadcast();
}

void UInventoryWidget::UpdateWidgetByItem(UItemBase* Item, bool Clear)
{
    OnUpdateWidgetsInfo.Broadcast(Item, Clear);
}

void UInventoryWidget::SetNotQuickInventoryVisibility(bool Hide)
{
    if (bIsInventoryHidden && Hide || !bIsInventoryHidden && !Hide) { return; }
    OnUpdateWidgetsVisibility.Broadcast(Hide, false);
    if (Hide && bIsShowingItemInfo) { ShowItemInfo(nullptr); }
    bIsInventoryHidden = Hide;
}

void UInventoryWidget::SetAllInventoryVisibility(bool Hide)
{
    if (bIsAllInventoryHidden && Hide || !bIsAllInventoryHidden && !Hide) { return; }
    OnUpdateWidgetsVisibility.Broadcast(Hide, true);
    bIsAllInventoryHidden = Hide;
}