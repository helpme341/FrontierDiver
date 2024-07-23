// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateWidgetsInfo, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateWidgetsUsability);
/*
* true = изменить все
* false = изменить только не быстрые виджеты
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateWidgetsVisibility, ESlateVisibility, SlateVisibility, bool, UpdateState);
DECLARE_LOG_CATEGORY_EXTERN(LogInventoryWidget, Log, All);

UCLASS()
class FRONTIERDIVER_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UInventoryWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UTexture2D* DefaultItemWidgetTexture;

    UFUNCTION(BlueprintCallable)
    UInventoryComponent* GetInventoryComponent() { return InventoryComponent.Get(); }

    UFUNCTION(BlueprintCallable)
    bool InventoryIsHidden() const { return bIsInventoryHidden; }

    UFUNCTION(BlueprintCallable)
    void SetNotQuickInventoryVisibility(bool Hide);

    UFUNCTION(BlueprintCallable)
    void SetAllInventoryVisibility(bool Hide);

    void UpdateWidgetsUsability();
    void UpdateWidgetByItem(UItemBase* Item, bool Clear);
    void UpdateAllWidgets();
    void ShowItemInfo(UItemBase* Item);
    void DropItemFromWidget(UItemBase* Item);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FUpdateWidgetsInfo OnUpdateWidgetsInfo;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FUpdateWidgetsUsability OnUpdateWidgetsUsability;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FUpdateWidgetsVisibility OnUpdateWidgetsVisibility;

    TStrongObjectPtr<UInventoryComponent> InventoryComponent;

protected:
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* ParentCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemQuantityTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescription;

    UPROPERTY()
    bool bIsWidgetsCreated = false;

    UPROPERTY()
    bool bIsInventoryHidden = false;

    UPROPERTY()
    bool bIsAllInventoryHidden = false;

    UPROPERTY()
    bool bIsShowingItemInfo = false;
};