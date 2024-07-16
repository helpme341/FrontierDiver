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
    StaticMesh->SetSimulatePhysics(true);
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

            if (FoundActor)
            {
                UDataTable* DataTable = FoundActor->FindDataTableByItemType(ItemType);

                if (DataTable)
                {
                    FItemTableRowInfoBase* ItemRow = DataTable->FindRow<FItemTableRowInfoBase>(ItemTypeName, "");

                    if (ItemRow)
                    {
                        if (StaticMesh && ItemRow->WorldItemStaticMesh)
                        {
                            StaticMesh->SetStaticMesh(ItemRow->WorldItemStaticMesh);
                            StaticMesh->SetMassOverrideInKg(NAME_None, ItemRow->WorldItemMass);
                            StaticMesh->SetWorldScale3D(ItemRow->WorldItemScale);
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("StaticMesh or ItemWorldStaticMesh is null."));
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("ItemRow not found for ItemTypeName: %s"), *ItemTypeName.ToString());
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("FoundActor is not of type AInventoryDataTableItemManager."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No AInventoryDataTableItemManager actors found in the world."));
        }
    }
}

bool AWorldItem::MainInteract(AFrontierDiverCharacter* Character)
{
	return Character->GetComponentByClass<UInventoryComponent>()->PickupItemToInventory(this);
}


void AWorldItem::LoadDataToWorldItem(FItemDynamicInfoBase& DynamicInfo, const FItemTableRowInfoBase* ItemTableRowInfo, TSubclassOf<UItemBase> NewItemType)
{
	ItemType = NewItemType;
	ItemDynamicInfo = DynamicInfo;
	StaticMesh->SetStaticMesh(ItemTableRowInfo->WorldItemStaticMesh);
    StaticMesh->SetWorldScale3D(ItemTableRowInfo->WorldItemScale);
    StaticMesh->SetMassOverrideInKg(NAME_None, ItemTableRowInfo->WorldItemMass);
}
