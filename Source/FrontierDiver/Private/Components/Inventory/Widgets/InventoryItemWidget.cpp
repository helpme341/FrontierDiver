// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
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
        WidgetButton->OnHovered.AddDynamic(this, &UInventoryItemWidget::OnButtonHovered);
        WidgetButton->OnUnhovered.AddDynamic(this, &UInventoryItemWidget::OnButtonUnhovered);
        WidgetButton->OnPressed.AddDynamic(this, &UInventoryItemWidget::OnButtonPressed);
    }
}

void UInventoryItemWidget::OnButtonPressed()
{
    if (Item)
    {
        InventoryWidget->DropItemFromWidget(Item.Get());
    }
}

void UInventoryItemWidget::OnButtonHovered()
{
    if (Item)
    {
        InventoryWidget->ShowItemInfo(Item.Get());
    }
}

void UInventoryItemWidget::OnButtonUnhovered()
{
    if (Item)
    {
        InventoryWidget->ShowItemInfo(nullptr);
    }
}

