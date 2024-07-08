// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/InventoryComponent.h"



UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}


void UInventoryWidget::LoadWidgestSlots()
{
    for (auto& Elem : InventoryComponent->Inventory)
    {
        Widgets.Add(Elem.Key);
        Widgets[Elem.Key].Array.SetNum(InventoryComponent->Inventory[Elem.Key].ContainerInventory.Num());
    }
}

void UInventoryWidget::CreateWidgets()
{
    for (auto& Elem : Widgets)
    {
        if (WidgetContainersSettings.Contains(Elem.Key))
        {
            const FWidgetContainerSettings& Settings = WidgetContainersSettings[Elem.Key];
            FVector2D CurrentPosition = Settings.ContainerStartPosition;
            int32 WidgetsInCurrentLine = 0;

            for (int32 Counter = 0; Counter < Elem.Value.Array.Num(); Counter++)
            {

                UInventoryItemWidget* NewWidget = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);

                NewWidget->WidgetCanvasPanel->RenderTransform.Scale = Settings.WidgetSize;

                UCanvasPanelSlot* CanvasSlot = ParentCanvasPanel->AddChildToCanvas(NewWidget);
                CanvasSlot->SetPosition(CurrentPosition);

                CanvasSlot->SetAnchors(FAnchors(Settings.MaximumAnchors, Settings.MaximumAnchors));


                if (WidgetsInCurrentLine == Settings.MaxWidgetsInOneLine)
                {
                    CurrentPosition.Y += Settings.ContainerSkipBetweenWidgets.Y;
                    CurrentPosition.X = Settings.ContainerStartPosition.X;
                    WidgetsInCurrentLine = 0;
                }
                else if (WidgetsInCurrentLine == 0)
                {
                    WidgetsInCurrentLine++;
                }
                else
                {
                    CurrentPosition.X += Settings.ContainerSkipBetweenWidgets.X;
                    WidgetsInCurrentLine++;
                }
                Widgets[Elem.Key].Array[Counter] = NewWidget;
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
            UpdateWidget(Elem.Value.ContainerInventory[Counter], Widgets[Elem.Key].Array[Counter]);
        }
    }
}

void UInventoryWidget::UpdateWidgetByItem(UItemBase* Item)
{
    UpdateWidget(Item, Widgets[Item->GetItemStaticInfo()->ItemContainerType].Array[Item->ThisItemID]);
}

void UInventoryWidget::UpdateWidget(UItemBase* Item, UInventoryItemWidget* ItemWidget)
{
    if (ItemWidget)
    {
        if (Item && ItemWidget->bIsThisEmptyWidget)
        {
            if (Item->bUseCustomUpdateWidgetForThisItem) { Item->UpdateItemWidget(ItemWidget); }
            else
            {
                ItemWidget->WidgetImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
                ItemWidget->WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), Item->GetItemDynamicInfo()->QuantityItems)));
                ItemWidget->LoadInformationAboutItem(Item);
            }
        }
        else if (!ItemWidget->bIsThisEmptyWidget)
        {
            ItemWidget->WidgetImage->SetBrushFromTexture(DefaultItemWidgetTexture);
            ItemWidget->WidgetTextBlock->SetText(FText());
            ItemWidget->ClearInformationAboutItem();
        }
    }
}

void UInventoryWidget::SetNotQuickInventoryVisibility(bool Hide)
{
    if (bIsInventoryHidden && Hide || !bIsInventoryHidden && !Hide) { return; }
    ESlateVisibility NewWidgetsState = Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    for (auto& ContainerSettings : WidgetContainersSettings)
    {
        if (!ContainerSettings.Value.bIsQuickInventory)
        {
            for (int32 Counter = 0; Counter < Widgets[ContainerSettings.Key].Array.Num(); Counter++)
            {
                if (Widgets[ContainerSettings.Key].Array[Counter])
                {
                    Widgets[ContainerSettings.Key].Array[Counter]->SetVisibility(NewWidgetsState);
                }
            }
        }
    }
    bIsInventoryHidden = Hide;
}

void UInventoryWidget::SetAllInvenotoryVisibility(bool Hide)
{
    if (bIsAllInventoryHidden && Hide || !bIsAllInventoryHidden && !Hide) { return; }
    ESlateVisibility NewWidgetsState = Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    for (auto& ContainerSettings : WidgetContainersSettings)
    {
        for (int32 Counter = 0; Counter < Widgets[ContainerSettings.Key].Array.Num(); Counter++)
        {
            if (Widgets[ContainerSettings.Key].Array[Counter])
            {
                Widgets[ContainerSettings.Key].Array[Counter]->SetVisibility(NewWidgetsState);
            }
        }
    }
    bIsAllInventoryHidden = Hide;
}
