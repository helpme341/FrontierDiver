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
#include "Components/Inventory/Widgets/DraggingItemWidget.h"


UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
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
        if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) { return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply; }
        else if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
        {
            InventoryWidget->DropItemFromWidget(Item.Get());
            return FReply::Handled();
        }
    }
    return Super::NativeOnMouseButtonDown(InGeometry, MouseEvent);
}

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    // Создаем объект DragDropOperation
    UInventoryDragDropOperation* DragDropOp = NewObject<UInventoryDragDropOperation>(this, InventoryWidget->InventoryDragDropOperation);
    UDraggingItemWidget* DraggingItemWidget = CreateWidget<UDraggingItemWidget>(this, InventoryWidget->DraggingItemWidgetClass);
    if (DragDropOp && DraggingItemWidget)
    {
        DragDropOp->Item = Item;
        DraggingItemWidget->UpdateWidgetInfo(Item->GetItemStaticInfo()->ItemWidgetTexture, WidgetTextBlock->Text);
        DragDropOp->DefaultDragVisual = DraggingItemWidget;
        SetItemInfoVisible(true);
        OutOperation = DragDropOp;
    }
}

bool UInventoryItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (UInventoryDragDropOperation* DragDropOperation = Cast<UInventoryDragDropOperation>(InOperation))
    {
        return InventoryWidget->InventoryComponent->MoveItemFromStartSlotToTargetSlot(*DragDropOperation->Item->ItemSlotInfo.Get(), WidgetSlotInfo);
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (UInventoryDragDropOperation* DragDropOperation = Cast<UInventoryDragDropOperation>(InOperation))
    {
        SetItemInfoVisible(false);
    }
}

FReply UInventoryItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) 
{
    if (!PlayerController) { PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); }
    FInputModeGameAndUI InputMode;
    PlayerController->SetInputMode(InputMode);
    return FReply::Handled();
}

void UInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (Item && bIsWidgetUsability) { InventoryWidget->ShowItemInfo(Item.Get()); }
}

void UInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) ////////////////
{
    if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton)
    {
        if (Item && bIsWidgetUsability) { InventoryWidget->ShowItemInfo(nullptr); }
    }
}

FReply UInventoryItemWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Tab) { return FReply::Handled(); }
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryItemWidget::SetItemInfoVisible(bool Hide)
{
    if (Hide && !bIsWidgetInfoInHedden)
    {
        WidgetImage->SetBrushFromTexture(InventoryWidget->DefaultItemWidgetTexture);
        WidgetTextBlock->SetVisibility(ESlateVisibility::Hidden);
        bIsWidgetInfoInHedden = true;
    }
    else if (!Hide && bIsWidgetInfoInHedden)
    {
        if (Item.IsValid() && Item->FindDataTableForItem(GetWorld()))
        {
            WidgetImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
            WidgetTextBlock->SetVisibility(ESlateVisibility::Visible);
            bIsWidgetInfoInHedden = false;
        }
    }
}

void UInventoryItemWidget::UpdateWidget(UItemBase* ItemRef, bool Clear)
{
    if (ItemRef && ItemRef->ItemSlotInfo->ID == WidgetSlotInfo.ID && ItemRef->ItemSlotInfo->ContainerType == WidgetSlotInfo.ContainerType)
    {
        if (ItemRef->FindDataTableForItem(GetWorld()))
        {
            if (Clear)
            {
                WidgetImage->SetBrushFromTexture(InventoryWidget->DefaultItemWidgetTexture);
                WidgetTextBlock->SetText(FText());
                Item->ItemWidget.Reset();
                Item.Reset();
            }
            else
            {
                Item.Reset(ItemRef);
                SetItemInfoVisible(false);
                if (!Item->ItemWidget.IsValid() || Item->ItemWidget.Get() != this) { Item->ItemWidget.Reset(this); }
                if (ItemRef->bUseCustomUpdateWidget) { ItemRef->CustomUpdateWidget(); }
                else
                {
                    WidgetImage->SetBrushFromTexture(ItemRef->GetItemStaticInfo()->ItemWidgetTexture);
                    //WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), ItemRef->GetItemDynamicInfo()->QuantityItems)));
                    WidgetTextBlock->SetText(FText::AsNumber(ItemRef->GetItemDynamicInfo()->QuantityItems));
                }
            }
        }
    }
}

void UInventoryItemWidget::SetWidgetUsability()
{
    TArray<FSharedContainerBase>& ContainerInventory = InventoryWidget->GetInventoryComponent()->Inventory[WidgetSlotInfo.ContainerType].ContainerInventory;

    if (ContainerInventory.IsValidIndex(WidgetSlotInfo.ID))
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
        if (!UpdateState && InventoryWidget->GetInventoryComponent()->QuickInventoryContainerType != WidgetSlotInfo.ContainerType) { SetVisibility(Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible); }
        else if (UpdateState) { SetVisibility(Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible); }
    }
}
