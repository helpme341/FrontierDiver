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
        // Get the outer widget of type UInventoryWidget
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
    if (Item->ItemID == WidgetID && Item->GetItemStaticInfo()->ItemContainerType == WidgetContainerType)
    {
        if (Item)
        {
            WidgetImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
            WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), Item->GetItemDynamicInfo()->QuantityItems)));
            Item.Reset(ItemRef);
            WidgetTextBlock->SetText(FText::AsNumber(Item->GetItemDynamicInfo()->QuantityItems));
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
    bIsWidetUsability = InventoryWidget->InventoryComponent->Inventory[WidgetContainerType].ContainerInventory.IsValidIndex(WidgetID);
    if (bIsWidetUsability)
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    else if (!bIsWidetUsability)
    {
        SetVisibility(ESlateVisibility::Visible);
    }
}

void UInventoryItemWidget::SetWidgetVisibility(ESlateVisibility SlateVisibility, bool UpdateState)
{
    if (bIsWidetUsability)
    {
        if (!UpdateState && !(InventoryWidget->InventoryComponent->QuickInventoryContainerType == WidgetContainerType))
        {
            SetVisibility(SlateVisibility);
        }
        else if (UpdateState && InventoryWidget->InventoryComponent->QuickInventoryContainerType == WidgetContainerType)
        {
            SetVisibility(SlateVisibility);
        }
    }
}

void UInventoryItemWidget::OnButtonPressed()
{
    if (Item && bIsWidetUsability)
    {
        InventoryWidget->DropItemFromWidget(Item.Get());
    }
}

void UInventoryItemWidget::OnButtonHovered()
{
    if (Item && bIsWidetUsability)
    {
        InventoryWidget->ShowItemInfo(Item.Get());
    }
}

void UInventoryItemWidget::OnButtonUnhovered()
{
    if (Item && bIsWidetUsability)
    {
        InventoryWidget->ShowItemInfo(nullptr);
    }
}
