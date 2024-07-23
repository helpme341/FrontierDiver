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

        WidgetButton->OnHovered.AddDynamic(this, &UInventoryItemWidget::OnButtonHovered);
        WidgetButton->OnUnhovered.AddDynamic(this, &UInventoryItemWidget::OnButtonUnhovered);
        WidgetButton->OnPressed.AddDynamic(this, &UInventoryItemWidget::OnButtonPressed);
    }
}

void UInventoryItemWidget::UpdateWidget(UItemBase* ItemRef)
{
    if (Item && Item->ItemID == WidgetID && Item->GetItemStaticInfo()->ItemContainerType == WidgetContainerType)
    {
        if (ItemRef)
        {
            WidgetImage->SetBrushFromTexture(ItemRef->GetItemStaticInfo()->ItemWidgetTexture);
            WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), ItemRef->GetItemDynamicInfo()->QuantityItems)));
            Item.Reset(ItemRef);
            WidgetTextBlock->SetText(FText::AsNumber(ItemRef->GetItemDynamicInfo()->QuantityItems));
            bIsThisEmptyWidget = false;
        }
        else if (!bIsThisEmptyWidget)
        {
            WidgetImage->SetBrushFromTexture(InventoryWidget->DefaultItemWidgetTexture);
            WidgetTextBlock->SetText(FText());
            Item = nullptr;
            bIsThisEmptyWidget = true;
        }
    }
}

void UInventoryItemWidget::SetWidgetUsability()
{
    UE_LOG(LogTemp, Warning, TEXT("SetWidgetUsability called for widget ID: %d"), WidgetID);
    bIsWidgetUsability = InventoryWidget->GetInventoryComponent()->Inventory[WidgetContainerType].ContainerInventory.IsValidIndex(WidgetID);
    if (bIsWidgetUsability)
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        SetVisibility(ESlateVisibility::Visible);
    }
}

void UInventoryItemWidget::SetWidgetVisibility(ESlateVisibility SlateVisibility, bool UpdateState)
{
    if (bIsWidgetUsability)
    {
        if (!UpdateState && !(InventoryWidget->GetInventoryComponent()->QuickInventoryContainerType == WidgetContainerType))
        {
            SetVisibility(SlateVisibility);
        }
        else if (UpdateState && InventoryWidget->GetInventoryComponent()->QuickInventoryContainerType == WidgetContainerType)
        {
            SetVisibility(SlateVisibility);
        }
    }
}

void UInventoryItemWidget::OnButtonPressed()
{
    if (Item && bIsWidgetUsability)
    {
        InventoryWidget->DropItemFromWidget(Item.Get());
    }
}

void UInventoryItemWidget::OnButtonHovered()
{
    if (Item && bIsWidgetUsability)
    {
        InventoryWidget->ShowItemInfo(Item.Get());
    }
}

void UInventoryItemWidget::OnButtonUnhovered()
{
    if (Item && bIsWidgetUsability)
    {
        InventoryWidget->ShowItemInfo(nullptr);
    }
}