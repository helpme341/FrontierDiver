// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryDataTableItemManager.generated.h"

UCLASS()
class FRONTIERDIVER_API AInventoryDataTableItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryDataTableItemManager();
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TMap<TSubclassOf<class UItemBase>, UDataTable*> DataTablesInfo;

	UFUNCTION(BlueprintCallable)
	UDataTable* FindDataTableByItemType(TSubclassOf<UItemBase> Item);
};
