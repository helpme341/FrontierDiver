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
    }
    InventoryWidget->GetInventoryComponent()->OnInventoryItemWidgetConstructed();
}

FReply UInventoryItemWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("Key Pressed: %s"), *InKeyEvent.GetKey().ToString());

    if (InKeyEvent.GetKey() == EKeys::Tab)
    {
        if (InventoryWidget && InventoryWidget->InventoryComponent)
        {
            auto OwnerCharacter = InventoryWidget->InventoryComponent->GetOwnerCharacter();
            if (OwnerCharacter)
            {
                UE_LOG(LogTemp, Warning, TEXT("Calling OpenCloseInventory"));
                OwnerCharacter->OpenCloseInventory();
                WidgetButton->SetKeyboardFocus();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is null"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("InventoryWidget or InventoryComponent is null"));
        }
        return FReply::Handled();
    }
    if (WidgetButton && WidgetButton->HasKeyboardFocus() && Item && bIsWidgetUsability)
    {
        if (InKeyEvent.GetKey() == EKeys::Q)
        {
            if (!bIsShowingItemInfo && WidgetButton->IsHovered())
            {
                InventoryWidget->ShowItemInfo(Item.Get());
                bIsShowingItemInfo = true;
                return FReply::Handled();
            }
        }
    }
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UInventoryItemWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (WidgetButton && WidgetButton->HasKeyboardFocus() && Item && bIsWidgetUsability)
    {
        if (InKeyEvent.GetKey() == EKeys::Q)
        {
            if (bIsShowingItemInfo && WidgetButton->IsHovered())
            {
                InventoryWidget->ShowItemInfo(nullptr);
                bIsShowingItemInfo = false;
                return FReply::Handled();
            }
        }
    }
    return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
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
    }
    return Super::NativeOnMouseButtonDown(InGeometry, MouseEvent);
}

FReply UInventoryItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    if (WidgetButton && WidgetButton->HasKeyboardFocus() && Item && bIsWidgetUsability)
    {
        if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
        {
            if (bIsShowingItemInfo && WidgetButton->IsHovered())
            {
                InventoryWidget->ShowItemInfo(nullptr);
                bIsShowingItemInfo = false;
                return FReply::Handled();
            }
        }
    }
    return Super::NativeOnMouseButtonUp(InGeometry, MouseEvent);
}

FReply UInventoryItemWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // Проверка, наведен ли курсор на кнопку
    if (WidgetButton && WidgetButton->IsHovered())
    {
        // Установка фокуса на кнопку
        WidgetButton->SetUserFocus(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    }
    // Вызов базовой реализации для продолжения обработки события перемещения мыши
    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
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

void UInventoryItemWidget::OnButtonUnhovered()
{
    if (Item && bIsWidgetUsability && bIsShowingItemInfo)
    {
        InventoryWidget->ShowItemInfo(nullptr);
        bIsShowingItemInfo = false;
    }
}