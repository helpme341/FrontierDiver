// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterfaceBase.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemClass : uint8
{
	None,
	Helmet,
	Costume,
	Sheets,
	Cylinder,
};


USTRUCT(BlueprintType)
struct FItemBase
{
	GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct FItemInfoBase : public  FItemBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryItemClass InventoryItemClass = EInventoryItemClass::None;
};


USTRUCT(BlueprintType)
struct FItemDataTibleBase : public FItemBase
{
	GENERATED_BODY()
};

UINTERFACE(MinimalAPI)
class UInventoryInterfaceBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IInventoryInterfaceBase : public UInterface
{
	GENERATED_BODY()
public:
};