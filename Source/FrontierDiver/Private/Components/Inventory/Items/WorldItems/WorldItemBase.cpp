// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/WorldItems/WorldItemBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/Base/InventoryItemClassBase.h"
#include "Components/Inventory/Items/Items/ItemBase.h"

// Sets default values
AWorldItemBase::AWorldItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

bool AWorldItemBase::AddThisItemToInventory(UInventoryComponent* Inventory)
{
	return false;
}
