// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/ItemsObjectTypes/InteractInventoryItemObjectBase.h"
#include "MainInventoryItemObjectBase.generated.h"

UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
	Outfit,
	b,
	c,
};

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UMainInventoryItemObjectBase : public UInteractInventoryItemObjectBase
{
	GENERATED_BODY()
	
};
