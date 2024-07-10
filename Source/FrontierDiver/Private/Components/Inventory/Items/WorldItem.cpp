// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/WorldItem.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/InventoryDataTableItemManager.h"


AWorldItem::AWorldItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
}

void AWorldItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemTypeName != "None")
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInventoryDataTableItemManager::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			AInventoryDataTableItemManager* FoundActor = Cast<AInventoryDataTableItemManager>(FoundActors[0]);
			FoundActor->FindDataTableByItemType(ItemType)->FindRow<FItemTableRowInfoBase>(ItemTypeName, "");
			StaticMesh->SetStaticMesh(FoundActor->FindDataTableByItemType(ItemType)->FindRow<FItemTableRowInfoBase>(ItemTypeName, "")->ItemWorldStaticMesh);
		}
	}
}

void AWorldItem::Interaction(AFrontierDiverCharacter* Character)
{
	Character->GetComponentByClass<UInventoryComponent>()->PickupItemToInventory(this);
}


void AWorldItem::LoadDataToWorldItem(FItemDynamicInfoBase& DynamicInfo, TSubclassOf<UItemBase> NewItemType, UStaticMesh* NewItemMesh)
{
	ItemType = NewItemType;
	ItemDynamicInfo = DynamicInfo;
	StaticMesh->SetStaticMesh(NewItemMesh);
}
