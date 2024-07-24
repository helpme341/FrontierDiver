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
    InventoryWidget->GetInventoryComponent()->OnInventoryItemWidgetConstructed();
}

void UInventoryItemWidget::UpdateWidget(UItemBase* ItemRef, bool Clear)
{
    if (ItemRef && ItemRef->ItemID == WidgetID && ItemRef->GetItemStaticInfo()->ItemContainerType == WidgetContainerType)
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
            Item = nullptr;
        }
    }
}

void UInventoryItemWidget::SetWidgetUsability()
{
    // Получаем инвентарь для данного типа контейнера
    TArray<FSharedContainerBase>& ContainerInventory = InventoryWidget->GetInventoryComponent()->Inventory[WidgetContainerType].ContainerInventory;

    // Проверяем, есть ли элемент с данным WidgetID в контейнере
    if (ContainerInventory.IsValidIndex(WidgetID))
    {
        // Если индекс действителен, показываем виджет
        SetVisibility(ESlateVisibility::Visible);
        bIsWidgetUsability = true;
    }
    else
    {
        // Если индекс недействителен, скрываем виджет
        SetVisibility(ESlateVisibility::Hidden);
        bIsWidgetUsability = false;
    }
}
void UInventoryItemWidget::SetWidgetVisibility(bool Hide, bool UpdateState)
{
    if (bIsWidgetUsability)
    {
        if (!UpdateState && InventoryWidget->GetInventoryComponent()->QuickInventoryContainerType != WidgetContainerType)
        {
            SetVisibility(Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible );
        }
        else if (UpdateState)
        {
            SetVisibility(Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
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