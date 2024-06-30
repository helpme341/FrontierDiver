// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/WorldItems/WorldItemBase.h"
#include "Components/Inventory/Items/Items/JewelryItem.h"
#include "JewelryWorldItem.generated.h"


/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API AJewelryWorldItem : public AWorldItemBase
{
	GENERATED_BODY()

public:

	FJewelryItemDynamicInfo ItemDynamicInfo;
};
