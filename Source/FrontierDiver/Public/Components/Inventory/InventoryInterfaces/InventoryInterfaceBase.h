// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterfaceBase.generated.h"

USTRUCT(BlueprintType)
struct FItemBase
{
	GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct FItemInfoBase
{
	GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct FItemDataTibleBase
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
class FRONTIERDIVER_API IInventoryInterfaceBase
{
	GENERATED_BODY()
public:
};