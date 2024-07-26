// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

UCLASS()
class FRONTIERDIVER_API UInventoryItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UInventoryItemWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
    virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EContainerType WidgetContainerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 WidgetID;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* WidgetCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* WidgetTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UImage* WidgetImage;

    UPROPERTY(meta = (BindWidget))
    class UButton* WidgetButton;

    UFUNCTION()
    void OnButtonUnhovered();

    UFUNCTION()
    void UpdateWidget(UItemBase* ItemRef, bool Clear);

    UFUNCTION()
    void SetWidgetUsability();

    UFUNCTION()
    void SetWidgetVisibility(bool Hide, bool UpdateState);

    TStrongObjectPtr<UItemBase> Item;

    class TStrongObjectPtr<UInventoryWidget> InventoryWidget;

    UPROPERTY()
    bool bIsWidgetUsability;

    bool bIsShowingItemInfo;
};