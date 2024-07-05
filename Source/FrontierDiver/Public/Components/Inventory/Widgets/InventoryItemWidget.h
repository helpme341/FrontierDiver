// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "InventoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public: 


	void SetWidgetState();

	void SetWidgetState(FItemTableRowInfoBase* ItemTableRowInfo);



	
};
