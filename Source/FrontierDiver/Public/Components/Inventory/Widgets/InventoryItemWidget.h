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

    void NativeConstruct() override;

    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
    FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
    //void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override; ////////////
    bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override; ////////////////

    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

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

    UFUNCTION()
    void UpdateWidget(UItemBase* ItemRef, bool Clear);

    UFUNCTION()
    void SetWidgetUsability();

    UFUNCTION()
    void SetWidgetVisibility(bool Hide, bool UpdateState);

    TStrongObjectPtr<UItemBase> Item;

    class TStrongObjectPtr<UInventoryWidget> InventoryWidget;

    class APlayerController* PlayerController;

    UPROPERTY()
    bool bIsWidgetUsability;
};