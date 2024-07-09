// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "InventoryWidget.generated.h"


USTRUCT(BlueprintType)
struct FWidgetContainerSettings
{                                                             
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D ContainerStartPosition = FVector2D(100.0f, 100.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D ContainerSkipBetweenWidgets = FVector2D(100.0f, 100.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
    FVector2D WidgetSize = FVector2D(1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anchors")
    FVector2D MinimumAnchors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anchors")
    FVector2D MaximumAnchors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsQuickInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxWidgetsInOneLine = 1;
};


USTRUCT(BlueprintType)
struct FWidgestContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UInventoryItemWidget*> Array;
};

class UInventoryComponent;
class UTextBlock;

UCLASS()
class FRONTIERDIVER_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UInventoryWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EContainerType, FWidgetContainerSettings> WidgetContainersSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UTexture2D* DefaultItemWidgetTexture;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* ParentCanvasPanel;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemQuantityTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescription;



    UFUNCTION(BlueprintCallable)
    class UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EContainerType, FWidgestContainer> Widgets;

    void UpdateWidgetByItem(UItemBase* Item, bool Clear);
    void UpdateAllWidgets();
    void UpdateWidget(UItemBase* Item, UInventoryItemWidget* ItemWidget);
    void CreateWidgets();
    void LoadWidgestSlots();
    void ShowItemInfo(UItemBase* Item);
    void DropItemFromWidget(UItemBase* Item);

    UFUNCTION(BlueprintCallable)
    void SetNotQuickInventoryVisibility(bool Hide);
    UFUNCTION(BlueprintCallable)
    void SetAllInvenotoryVisibility(bool Hide);

    bool bIsWidgetsCreated;
    bool bIsInventoryHidden = true;
    bool bIsAllInventoryHidden;
    bool bIsShowingItemInfo;

    UInventoryComponent* InventoryComponent;
};