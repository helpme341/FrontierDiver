// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldItemBase.generated.h"

class UInventoryComponent;
class UItemBase;

UCLASS()
class FRONTIERDIVER_API AWorldItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItemBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	 UItemBase* Item;

	virtual bool AddThisItemToInventory(UInventoryComponent* Inventory);

};
