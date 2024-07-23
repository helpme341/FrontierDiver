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
    Slot,
    ClothingOne,
    ClothingTwo,
    Array,
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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    UTexture2D* ItemWidgetTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    EContainerType ItemContainerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 MaxQuantityItemsInSlot = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings|WorldItem")
    UStaticMesh* WorldItemStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings|WorldItem")
    float WorldItemMass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings|WorldItem")
    FVector WorldItemScale = FVector(1.0f,1.0f,1.0f);
};

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

    bool bIsPlayerCanDropThisItem;

    int32 ItemID = 99; 

    virtual bool FindDataTableByItemType(UWorld* World) { return false; };

    virtual void OnPickupItemToInventory(class AWorldItem* Item) {};

    virtual const FItemTableRowInfoBase* GetItemStaticInfo() { return nullptr; }

    virtual UItemDynamicInfo* GetItemDynamicInfo() { return nullptr; }

    virtual void SetItemDynamicInfo(UItemDynamicInfo* DynamicInfo) {};

protected:
    /*FT == ItemTableRowInfo*/
    template<typename T, typename FT = FItemTableRowInfoBase>
    bool BaseFindDataTableByItemType(UWorld* World);
};

template<typename T, typename FT>
inline bool UItemBase::BaseFindDataTableByItemType(UWorld* World) // переписать 
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
