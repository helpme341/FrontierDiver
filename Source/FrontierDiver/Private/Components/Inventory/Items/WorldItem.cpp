// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/WorldItem.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/InventoryComponent.h"

AWorldItem::AWorldItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
}

void AWorldItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemTypeName != "None")
	{
		if (AFrontierDiverCharacter* Character = Cast<AFrontierDiverCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			if (UDataTable* ItemDataTable = Character->GetComponentByClass<UInventoryComponent>()->FindDataTableByItemType(ItemType))
			{
				if (FItemTableRowInfoBase* TableRow = ItemDataTable->FindRow<FItemTableRowInfoBase>(ItemTypeName, ""))
				{
					StaticMesh->SetStaticMesh(TableRow->ItemWorldStaticMesh);
				}
			}
		}
	}
}

void AWorldItem::Interaction(AFrontierDiverCharacter* Character)
{
	Character->GetComponentByClass<UInventoryComponent>()->PickupItemToInventory(this);
}


void AWorldItem::LoadDataToWorldItem(FItemDynamicInfoBase DynamicInfo, TSubclassOf<UItemBase> NewItemType, UStaticMesh* NewItemMesh)
{
	ItemType = NewItemType;
	ItemDynamicInfo = DynamicInfo;
	StaticMesh->SetStaticMesh(NewItemMesh);
}
