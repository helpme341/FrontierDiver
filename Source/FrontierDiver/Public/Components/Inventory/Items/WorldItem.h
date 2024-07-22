// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Interfaces/InteractionIF.h"
#include "Components/SceneComponent.h"
#include "WorldItem.generated.h"

class UItemBase;

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API AWorldItem : public AActor, public IInteractionIF
{
	GENERATED_BODY()

public:

	AWorldItem();

	virtual void OnConstruction(const FTransform& Transform) override;

	bool MainInteract(AFrontierDiverCharacter* Character) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemTypeName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemBase> ItemType;

	TStrongObjectPtr<UItemDynamicInfo> ItemDynamicInfo;

	void LoadDataToWorldItem(UItemDynamicInfo* ItemDynamic, const FItemTableRowInfoBase* temTableRowInfo, TSubclassOf<UItemBase> NewItemType);

private:

	class TStrongObjectPtr<AInventoryDataTableItemManager> FoundActor;
};