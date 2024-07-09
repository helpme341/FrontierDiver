// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Character/Interfaces/InteractionIF.h"
#include "WorldItem.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API AWorldItem : public AActor, public IInteractionIF
{
	GENERATED_BODY()

public:

	AWorldItem();

	void BeginPlay() override;

	void Interaction(AFrontierDiverCharacter* Character) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemTypeName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemBase> ItemType;

	FItemDynamicInfoBase ItemDynamicInfo;

	void LoadDataToWorldItem(FItemDynamicInfoBase& DynamicInfo, TSubclassOf<UItemBase> NewItemType, UStaticMesh* NewItemMesh);

};
