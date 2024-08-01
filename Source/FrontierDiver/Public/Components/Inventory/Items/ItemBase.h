// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Inventory/InventoryDataTableItemManager.h"
#include "Character/FrontierDiverCharacter.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EContainerType : uint8
{
    None,
    Clothing,
    Flippers,
    BreathingTanks,
    QuickInventory,
};

UCLASS()
class FRONTIERDIVER_API UItemDynamicInfo : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    FName ItemTypeName = "None";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 QuantityItems = 1;
};

USTRUCT(BlueprintType)
struct FHeldItemInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeldItemInfo")
    FName ItemSocketName = "None";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeldItemInfo")
    FTransform  ItemItemAttachOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeldItemInfo")
    EAnimItemBlendType ItemAnimBlendType;
};

USTRUCT(BlueprintType)
struct FItemTableRowInfoBase : public FTableRowBase
{
    GENERATED_BODY()

    FItemTableRowInfoBase()
        : ItemWidgetTexture(nullptr),
        WorldItemStaticMesh(nullptr),
        WorldItemMass(0.0f)
    {}
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    UTexture2D* ItemWidgetTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    FText ItemDescription;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    TSet<EContainerType> ItemContainerTypes = {
        EContainerType::QuickInventory
    };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 MaxQuantityItemsInSlot = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings|WorldItem")
    UStaticMesh* WorldItemStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings|WorldItem")
    float WorldItemMass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings|WorldItem")
    FVector WorldItemScale = FVector(1.0f,1.0f,1.0f);


    bool bUseAutoStaking; ////////////////
};

USTRUCT(BlueprintType)
struct FSlotInfo
{
    GENERATED_BODY()


    FSlotInfo(EContainerType ContainerType, int32 ID)
    {
        FSlotInfo::ID = ID;
        FSlotInfo::ContainerType = ContainerType;
    }


    FSlotInfo() {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfoBase")
    int32 ID = 99;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfoBase")
    EContainerType ContainerType = EContainerType::None;

    bool IsValid() { return (ID != -1 && ContainerType != EContainerType::None); }

    bool operator==(const FSlotInfo& Other) const
    {
        return ID == Other.ID && ContainerType == Other.ContainerType;
    }

    bool operator!=(const FSlotInfo& Other) const
    {
        return !(*this == Other);
    }

};

class UInventoryItemWidget;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

    bool bIsPlayerCanDropThisItem;

    bool bUseCustomUpdateWidget;

    bool bUseCustomShowItemInfo;

    bool bUseCustomUpdateItemInfo;

    TSharedPtr<FSlotInfo> ItemSlotInfo = MakeShared<FSlotInfo>();

    TStrongObjectPtr<UInventoryItemWidget> ItemWidget;

    virtual bool FindDataTableForItem(UWorld* World) { return false; };

    virtual void OnPickupItemToInventory(class AWorldItem* Item) {};

    virtual void OnAddItemToInventory() {};

    virtual void OnRemoveItemFromInventory() {};

    virtual void CustomUpdateWidget() {};

    virtual void CustomShowItemInfo() {};

    virtual void CustomUpdateItemInfo() {};

    virtual const FItemTableRowInfoBase* GetItemStaticInfo() { return nullptr; }

    virtual UItemDynamicInfo* GetItemDynamicInfo() { return nullptr; }

    virtual void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) {};

protected:
    /*FT == ItemTableRowInfo*/
    template<typename T, typename FT = FItemTableRowInfoBase>
    bool BaseFindDataTableForItem(UWorld* World);
};

template<typename T, typename FT>
inline bool UItemBase::BaseFindDataTableForItem(UWorld* World) // переписать 
{
    T* Item = Cast<T>(this);
    if (!Item->ItemTableRowInfo.IsValid() && Item->ItemDynamicInfo->ItemTypeName != "None")
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(World, AInventoryDataTableItemManager::StaticClass(),FoundActors);
        if (FoundActors.Num() > 0)
        {
            AInventoryDataTableItemManager* FoundActor = Cast<AInventoryDataTableItemManager>(FoundActors[0]);
            if (UDataTable* DataTable =  FoundActor->FindDataTableByItemType(T::StaticClass()))
            {
                if (FT* FTRef = DataTable->FindRow<FT>(Item->ItemDynamicInfo->ItemTypeName, ""))
                {
                    Item->ItemTableRowInfo = MakeShared<FT>(*FTRef);
                    return true;
                }
            }
            return false;
        }
    }
    else if (Item->ItemTableRowInfo.IsValid()) { return true; }
    return false;
}
