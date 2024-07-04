// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "WorldItem.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API AWorldItem : public AActor
{
	GENERATED_BODY()

public:

	AWorldItem();

	void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemTypeName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemBase> ItemType;

	FItemDynamicInfoBase ItemDynamicInfo;

	void LoadDataToWorldItem(FItemDynamicInfoBase DynamicInfo, TSubclassOf<UItemBase> NewItemType, UStaticMesh* NewItemMesh);

};
