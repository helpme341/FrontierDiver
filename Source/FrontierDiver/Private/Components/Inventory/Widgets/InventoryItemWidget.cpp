// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Widgets/InventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Inventory/Widgets/DragItemWidget.h"

UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsFocusable = true;
    SetIsEnabled(true);
}

void UInventoryItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InventoryWidget.Reset(GetTypedOuter<UInventoryWidget>());
    if (InventoryWidget)
    {
        InventoryWidget->OnUpdateWidgetsInfo.AddUObject(this, &UInventoryItemWidget::UpdateWidget);
        InventoryWidget->OnUpdateWidgetsUsability.AddUObject(this, &UInventoryItemWidget::SetWidgetUsability);
        InventoryWidget->OnUpdateWidgetsVisibility.AddUObject(this, &UInventoryItemWidget::SetWidgetVisibility);
    }
    InventoryWidget->GetInventoryComponent()->OnInventoryItemWidgetConstructed();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    if (Item && bIsWidgetUsability)
    {
        if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
        {
            InventoryWidget->DropItemFromWidget(Item.Get());
            return FReply::Handled();
        }
    }
    
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        InventoryWidget->DropItemFromWidget(Item.Get());
        return FReply::Handled();
        //UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass()));
        //if (DragDropOp)
        //{
        //    UDragItemWidget* DraggedWidget = CreateWidget<UDragItemWidget>(GetWorld(), InventoryWidget->DragItemWidget);
        //    DragDropOp->Item = Item;
        //    DragDropOp->DefaultDragVisual = DraggedWidget;
        //}
        //return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, MouseEvent);
}

bool UInventoryItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (UInventoryDragDropOperation* DragDropOperation = Cast<UInventoryDragDropOperation>(InOperation))
    {
        //InventoryWidget->InventoryComponent->AddItemToInventory()
        return true; // ¬озвращаем true, если операци€ была успешной
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

FReply UInventoryItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    if (!PlayerController) { PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); }
    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);
    return FReply::Handled();
}

void UInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (Item && bIsWidgetUsability) { InventoryWidget->ShowItemInfo(Item.Get()); }
}

void UInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    if (Item && bIsWidgetUsability) { InventoryWidget->ShowItemInfo(nullptr); }
}

FReply UInventoryItemWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Tab)
    {
        return FReply::Handled();
    }
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryItemWidget::UpdateWidget(UItemBase* ItemRef, bool Clear)
{
    if (ItemRef && ItemRef->ItemID == WidgetID && ItemRef->ItemContainerType == WidgetContainerType)
    {
        if (Clear)
        {
            WidgetImage->SetBrushFromTexture(InventoryWidget->DefaultItemWidgetTexture);
            WidgetTextBlock->SetText(FText());
            Item.Reset();
            Item->ItemWidget.Reset();
        }
        else
        {
            WidgetImage->SetBrushFromTexture(ItemRef->GetItemStaticInfo()->ItemWidgetTexture);
            WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), ItemRef->GetItemDynamicInfo()->QuantityItems)));
            Item.Reset(ItemRef);// ItemWidget
            WidgetTextBlock->SetText(FText::AsNumber(ItemRef->GetItemDynamicInfo()->QuantityItems));
            if (!Item->ItemWidget.IsValid()) { Item->ItemWidget.Reset(this); }
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
