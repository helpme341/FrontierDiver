// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:

	class  TStrongObjectPtr<UItemBase> Item;
};
