// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UDragItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WidgetTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UImage* WidgetImage;

	void SetDragItemWidgetInfo(class UItemBase* Item);
};
