// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/CanvasPanel.h"
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

    TArray<UInventoryItemWidget*> Array;
};

class UInventoryComponent;

UCLASS()
class FRONTIERDIVER_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UInventoryWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EContainerType, FWidgetContainerSettings> WidgetContainersSettings;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* ParentCanvasPanel;

    UFUNCTION(BlueprintCallable)
    UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;

    TMap<EContainerType, FWidgestContainer> Widgets;

    void UpdateWidgetByID(EContainerType ContainerType, int32 ItemID, FItemTableRowInfoBase* ItemTableRowInfo);
    void UpdateAllWidgets();
    void UpdateWidget(UItemBase* Item, UInventoryItemWidget* ItemWidget);
    void CreateWidgets();
    void LoadWidgestSlots();

    UFUNCTION(BlueprintCallable)
    void SetQuickInventoryVisibility(bool Hide);
    UFUNCTION(BlueprintCallable)
    void SetAllInvenotoryVisibility(bool Hide);

    bool bIsWidgetsCreated;
    bool bIsInventoryHidden;
    bool bIsAllInventoryHidden;

    UInventoryComponent* InventoryComponent;
};