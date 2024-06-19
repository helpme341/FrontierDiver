// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/Inventory/InventoryInterfaces/InventoryInterfaceBase.h"
#include "MainInventoryInterfaceBase.generated.h"


UENUM(BlueprintType)
enum class EMainInventorySlotType : uint8
{
	None,
	Helmet,
	Costume,
	Sheets,
};

USTRUCT(BlueprintType)
struct FMainInventoryItemDataTibleBase : public FItemDataTibleBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EMainInventorySlotType>  MainInventorySlotTypes;
};

UINTERFACE(MinimalAPI)
class UMainInventoryInterfaceBase : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class FRONTIERDIVER_API IMainInventoryInterfaceBase
{
	GENERATED_BODY()
};
