// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Widgets/DraggingItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UDraggingItemWidget::UpdateWidgetInfo(UTexture2D* ItemTexture, FText& ItemQuantity)
{
	WidgetImage->SetBrushFromTexture(ItemTexture);
	WidgetTextBlock->SetText(ItemQuantity);
}
