// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainWidget.h"
#include "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"
#include "Components/TextBlock.h"

UMainWidget::UMainWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = false;
}

void UMainWidget::NativeConstruct()
{
}

void UMainWidget::UpdateWidgetForAir()
{
	FString AmountString = FString::Printf(TEXT("%.1f/%.1f"), DiverCharacter->GetGlobalCurrentAir(), DiverCharacter->GetGlobalMaxAir());
	WidgetAirTextBlock->SetText(FText::FromString(AmountString));
}
