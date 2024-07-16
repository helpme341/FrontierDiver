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
    StaticMesh->SetLinearDamping(2.0f);
    StaticMesh->SetAngularDamping(1.0f);
}

void AWorldItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (ItemTypeName != "None")
    {
        if (!FoundActor)
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInventoryDataTableItemManager::StaticClass(), FoundActors);
            
            if (FoundActors.Num() > 0) { FoundActor = Cast<AInventoryDataTableItemManager>(FoundActors[0]); }
            else { return; }
        }


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
                    if (FoundActor->WaterPhysicalMaterial) { StaticMesh->SetPhysMaterialOverride(FoundActor->WaterPhysicalMaterial); }
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
    if (FoundActor && FoundActor->WaterPhysicalMaterial) { StaticMesh->SetPhysMaterialOverride(FoundActor->WaterPhysicalMaterial); }
    StaticMesh->SetMassOverrideInKg(NAME_None, ItemTableRowInfo->WorldItemMass);
}
