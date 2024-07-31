// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggingItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UDraggingItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    void UpdateWidgetInfo(UTexture2D* ItemTexture, FText& ItemQuantity);

private:

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* WidgetTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UImage* WidgetImage;
};
