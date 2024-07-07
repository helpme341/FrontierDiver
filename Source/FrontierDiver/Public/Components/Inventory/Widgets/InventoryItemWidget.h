// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "InventoryItemWidget.generated.h"


/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public: 

	UInventoryItemWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* WidgetCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WidgetTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* WidgetButton;

	UPROPERTY(meta = (BindWidget))
	UImage* WidgetImage;

	void SetWidgetState();
};
