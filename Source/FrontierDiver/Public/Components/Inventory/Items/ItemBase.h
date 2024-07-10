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

USTRUCT(BlueprintType)
struct FItemDynamicInfoBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    FName ItemTypeName = "None";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 QuantityItems = 1;

};

USTRUCT(BlueprintType)
struct FItemTableRowInfoBase : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    UTexture2D* ItemWidgetTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    UStaticMesh* ItemWorldStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    EContainerType ItemContainerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
    int32 MaxQuantityItemsInSlot = 1;
};


/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

    int32 ThisItemID = 99;

    virtual bool FindDataTableByItemType();

    virtual FItemTableRowInfoBase* GetItemStaticInfo() { return nullptr; }

    virtual FItemDynamicInfoBase& GetItemDynamicInfo() { static FItemDynamicInfoBase DummyDynamicInfo; return DummyDynamicInfo; }

    bool bUseCustomAddThisItemToInventory = false;

    bool bUseCustomRemoveThisItemFromInventory = false;

    bool bUseCustomUpdateWidgetForThisItem = false;

    virtual void UpdateItemWidget(class UInventoryItemWidget* ItemWidget);

	virtual bool AddThisItemToInventory(UInventoryComponent* Inventory);

	virtual bool RemoveThisItemFromInventory(UInventoryComponent* Inventory, bool DestroyItem);

protected:

    template<typename T, typename FT>
    bool BaseFindDataTableByItemType();
};

template<typename T, typename FT>
inline bool UItemBase::BaseFindDataTableByItemType()
{
    T* Item = Cast<T>(this);
    if (!Item->ItemTableRowInfo && Item->ItemDynamicInfo.ItemTypeName != "None")
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInventoryDataTableItemManager::StaticClass(),FoundActors);
        if (FoundActors.Num() > 0)
        {
            AInventoryDataTableItemManager* FoundActor = Cast<AInventoryDataTableItemManager>(FoundActors[0]);
            Item->ItemTableRowInfo = FoundActor->FindDataTableByItemType(T::StaticClass())->FindRow<FT>(Item->ItemDynamicInfo.ItemTypeName, "");
            return true;
        }
    }
    else if (Item->ItemTableRowInfo) { return true; }
    return false;
}
