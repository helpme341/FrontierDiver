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
    if (ItemID != -1)
    {
        InventoryWidget->DropItemFromWidget(InventoryWidget->GetInventoryComponent()->Inventory[ItemContainerType].ContainerInventory[ItemID]);
    }
}

void UInventoryItemWidget::OnButtonHovered()
{
    if (ItemID != -1)
    {
        InventoryWidget->ShowItemInfo(InventoryWidget->GetInventoryComponent()->Inventory[ItemContainerType].ContainerInventory[ItemID]);
    }
}

void UInventoryItemWidget::OnButtonUnhovered()
{
    if (ItemID != -1)
    {
        InventoryWidget->ShowItemInfo(nullptr);
    }
}

