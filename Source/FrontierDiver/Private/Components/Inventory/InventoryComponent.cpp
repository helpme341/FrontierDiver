// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "Engine/StaticMeshActor.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


bool UInventoryComponent::AddItemToInventory(FItemInfoBase ItemInfo)
{
	switch (ItemInfo.InventoryItemClass)
	{
	case EInventoryItemClass::Cylinder:
		// Пример явного преобразования
		if (ItemInfo.InventoryItemClass == EInventoryItemClass::Cylinder)
		{
			const FInventoryCylinderInfo& CylinderInfo = dynamic_cast<const FInventoryCylinderInfo&>(ItemInfo);
			// Делаем что-то с CylinderInfo
			// Например, сохраняем его в вашем инвентаре или выполняем другие операции
		}
		break;
	default:
		break;
	}
	return false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	CastToOwners();
}
