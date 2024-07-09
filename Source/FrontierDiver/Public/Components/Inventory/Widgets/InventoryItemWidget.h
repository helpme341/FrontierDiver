// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* WidgetCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WidgetTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UImage* WidgetImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* WidgetButton;


	UFUNCTION()
	void OnButtonPressed();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();

	class UItemBase* Item;

	class UInventoryWidget* InventoryWidget;

	bool bIsThisEmptyWidget = true;
};
