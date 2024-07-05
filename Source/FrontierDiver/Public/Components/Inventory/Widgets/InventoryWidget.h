// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/CanvasPanel.h"
#include "InventoryWidget.generated.h"

USTRUCT(BlueprintType)
struct FWidgestContainerSettings
{                                                             
    GENERATED_BODY()

    FVector2D ContainerStartPosition = FVector2D(100.0f, 100.0f);

    bool bIsQuickInventory;

    FVector2D ContainerSkipBetweenWidgets = FVector2D(100.0f, 100.0f);

    int32 MaxWidgesInOneLine = 1;
};

USTRUCT(BlueprintType)
struct FWidgestContainer
{
    GENERATED_BODY()

    TArray<UInventoryItemWidget*> Array;
};

UCLASS()
class FRONTIERDIVER_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UInventoryWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<EContainerType, FWidgestContainerSettings> WidgestContainersSettings;
    TMap<EContainerType, FWidgestContainer> Widgests;

    void UpdateWidgetByID(EContainerType ContainerType, int32 ItemID, FItemTableRowInfoBase* ItemTableRowInfo);
    void UpdateAllWidgets();
    void UpdateWidget(UItemBase* Item, UInventoryItemWidget* ItemWidget);
    void SetQuickInventoryVisibility(bool Hide);
    void CreateWidgests();
    void SetAllInvenotoryVisibility(bool Hide);

    bool bIsWidgestsCreated;
    bool bIsInventoryHidden;
    bool bIsAllInventoryHidden;

private:
    UInventoryComponent* InventoryComponent;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCanvasPanel* ParentCanvasPanel;

    UFUNCTION(BlueprintCallable)
    UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;
};