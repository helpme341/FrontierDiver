// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Inventory/InventoryComponent.h"

UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UInventoryItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WidgetButton)
    {
        InventoryWidget.Reset(GetTypedOuter<UInventoryWidget>());

        if (InventoryWidget)
        {
            InventoryWidget->OnUpdateWidgetsInfo.AddDynamic(this, &UInventoryItemWidget::UpdateWidget);
            InventoryWidget->OnUpdateWidgetsUsability.AddDynamic(this, &UInventoryItemWidget::SetWidgetUsability);
            InventoryWidget->OnUpdateWidgetsVisibility.AddDynamic(this, &UInventoryItemWidget::SetWidgetVisibility);
        }
        WidgetButton->OnUnhovered.AddDynamic(this, &UInventoryItemWidget::OnButtonUnhovered);
        WidgetButton->OnHovered.AddDynamic(this, &UInventoryItemWidget::OnButtonHovered);
    }
    InventoryWidget->GetInventoryComponent()->OnInventoryItemWidgetConstructed();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    if (WidgetButton && Item && bIsWidgetUsability)
    {
        if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
        {
            InventoryWidget->DropItemFromWidget(Item.Get());
            return FReply::Handled();
        }
        else if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
        {
            // логика взятия придмета в руки
            return FReply::Handled();
        }
    }
    return Super::NativeOnMouseButtonDown(InGeometry, MouseEvent);
}

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

void UInventoryItemWidget::UpdateWidget(UItemBase* ItemRef, bool Clear)
{
    if (ItemRef && ItemRef->ItemID == WidgetID && ItemRef->ItemContainerType == WidgetContainerType)
    {
        if (!Clear)
        {
            WidgetImage->SetBrushFromTexture(ItemRef->GetItemStaticInfo()->ItemWidgetTexture);
            WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), ItemRef->GetItemDynamicInfo()->QuantityItems)));
            Item.Reset(ItemRef);
            WidgetTextBlock->SetText(FText::AsNumber(ItemRef->GetItemDynamicInfo()->QuantityItems));
        }
        else
        {
            WidgetImage->SetBrushFromTexture(InventoryWidget->DefaultItemWidgetTexture);
            WidgetTextBlock->SetText(FText());
            Item.Reset();
        }
    }
}

void UInventoryItemWidget::SetWidgetUsability()
{
    TArray<FSharedContainerBase>& ContainerInventory = InventoryWidget->GetInventoryComponent()->Inventory[WidgetContainerType].ContainerInventory;

    if (ContainerInventory.IsValidIndex(WidgetID))
    {
        SetVisibility(ESlateVisibility::Visible);
        bIsWidgetUsability = true;
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
        bIsWidgetUsability = false;
    }
}

void UInventoryItemWidget::SetWidgetVisibility(bool Hide, bool UpdateState)
{
    if (bIsWidgetUsability)
    {
        if (!UpdateState && InventoryWidget->GetInventoryComponent()->QuickInventoryContainerType != WidgetContainerType) { SetVisibility(Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible); }
        else if (UpdateState) { SetVisibility(Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible); }
    }
}

void UInventoryItemWidget::OnButtonHovered()
{
    if (Item && bIsWidgetUsability) { InventoryWidget->ShowItemInfo(Item.Get()); }
}

void UInventoryItemWidget::OnButtonUnhovered()
{
    if (Item && bIsWidgetUsability) { InventoryWidget->ShowItemInfo(nullptr); }
}