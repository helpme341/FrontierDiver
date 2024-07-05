// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"



UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ParentCanvasPanel = CreateDefaultSubobject<UCanvasPanel>(TEXT("ParentCanvasPanel"));
}

void UInventoryWidget::UpdateWidgetByID(EContainerType ContainerType, int32 ItemID, FItemTableRowInfoBase* ItemTableRowInfo)
{
    Widgests[ContainerType].Array[ItemID]->SetWidgetState(ItemTableRowInfo);
}

void UInventoryWidget::UpdateAllWidgets()
{
    for (auto& Elem : InventoryComponent->Inventory)
    {
        for (int32 Counter = 0; Counter < Elem.Value.ContainerInventory.Num(); Counter++)
        {
            UpdateWidget(Elem.Value.ContainerInventory[Counter], Widgests[Elem.Key].Array[Counter]);
        }
    }
}

void UInventoryWidget::UpdateWidget(UItemBase* Item, UInventoryItemWidget* ItemWidget)
{
    if (Item)
    {
        ItemWidget->SetWidgetState(Item->GetItemStaticInfo());
    }
    else
    {
        ItemWidget->SetWidgetState();
    }
}

void UInventoryWidget::SetQuickInventoryVisibility(bool Hide)
{
    if (bIsInventoryHidden && Hide || !bIsInventoryHidden && !Hide) { return; }
    ESlateVisibility NewWidgetsState = Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    for (auto& ContainerSettings : WidgestContainersSettings)
    {
        if (!ContainerSettings.Value.bIsQuickInventory)
        {
            for (int32 Counter = 0; Counter < Widgests[ContainerSettings.Key].Array.Num(); Counter++)
            {
                if (Widgests[ContainerSettings.Key].Array[Counter])
                {
                    Widgests[ContainerSettings.Key].Array[Counter]->SetVisibility(NewWidgetsState);
                }
            }
        }
    }
    bIsInventoryHidden = Hide;
}

void UInventoryWidget::CreateWidgests()
{
    for (auto& Elem : Widgests)
    {
        const FWidgestContainerSettings& Settings = WidgestContainersSettings[Elem.Key];
        FVector2D CurrentPosition = Settings.ContainerStartPosition;
        int32 WidgetsInCurrentLine = 0;

        for (int32 Counter = 0; Counter < Elem.Value.Array.Num(); Counter++)
        {
            /**
            UInventoryItemWidget* NewWidget = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);
            if (NewWidget)
            {
                if (WidgetsInCurrentLine == 0)
                {

                    WidgetsInCurrentLine++;
                }


                UCanvasPanelSlot* CanvasSlot = ParentCanvasPanel->AddChildToCanvas(NewWidget);
                if (CanvasSlot)
                {
                    CanvasSlot->SetPosition(CurrentPosition);
                    CanvasSlot->SetSize(FVector2D(100, 100)); // Adjust size as needed
                }

                Widgests[Elem.Key].Array[Counter] = NewWidget;

                WidgetsInCurrentLine++;
                if (WidgetsInCurrentLine >= Settings.MaxWidgesInOneLine)
                {
                    CurrentPosition.X = Settings.ContainerStartPosition.X;
                    CurrentPosition.Y += Settings.ContainerSkipBetweenWidgets.Y;
                    WidgetsInCurrentLine = 0;
                }
                else
                {
                    CurrentPosition.X += Settings.ContainerSkipBetweenWidgets.X;
                }
            }
            */
        }
    }
}

void UInventoryWidget::SetAllInvenotoryVisibility(bool Hide)
{
    if (bIsAllInventoryHidden && Hide || !bIsAllInventoryHidden && !Hide) { return; }
    ESlateVisibility NewWidgetsState = Hide ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    for (auto& ContainerSettings : WidgestContainersSettings)
    {
        for (int32 Counter = 0; Counter < Widgests[ContainerSettings.Key].Array.Num(); Counter++)
        {
            if (Widgests[ContainerSettings.Key].Array[Counter])
            {
                Widgests[ContainerSettings.Key].Array[Counter]->SetVisibility(NewWidgetsState);
            }
        }
    }
    bIsAllInventoryHidden = Hide;
}
