// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Widgets/DragItemWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UDragItemWidget::SetDragItemWidgetInfo(UItemBase* Item)
{
	WidgetImage->SetBrushFromTexture(Item->GetItemStaticInfo()->ItemWidgetTexture);
	WidgetTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%03d"), Item->GetItemDynamicInfo()->QuantityItems)));
}
