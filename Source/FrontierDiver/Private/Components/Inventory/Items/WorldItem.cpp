// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/WorldItem.h"
#include "Kismet/GameplayStatics.h"
#include  "Character/FrontierDiverCharacter.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/InventoryDataTableItemManager.h"
#include "Kismet/GameplayStatics.h"

AWorldItem::AWorldItem()
{
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = SceneComponent;
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMesh->SetupAttachment(RootComponent);
    StaticMesh->SetSimulatePhysics(true);
    StaticMesh->SetLinearDamping(2.0f);
    StaticMesh->SetAngularDamping(1.0f);
}

void AWorldItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (ItemTypeName != "None")
    {
        if (!FoundActor.IsValid())
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInventoryDataTableItemManager::StaticClass(), FoundActors);

            if (FoundActors.Num() > 0)
            {
                FoundActor = Cast<AInventoryDataTableItemManager>(FoundActors[0]);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No AInventoryDataTableItemManager actors found"));
                return;
            }
        }

        AInventoryDataTableItemManager* ValidFoundActor = FoundActor.Get();
        if (ValidFoundActor)
        {
            UDataTable* DataTable = ValidFoundActor->FindDataTableByItemType(ItemType);

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
                        if (ValidFoundActor->WaterPhysicalMaterial)
                        {
                            StaticMesh->SetPhysMaterialOverride(ValidFoundActor->WaterPhysicalMaterial);
                        }
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
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("DataTable not found for ItemType: %s"), *ItemType->GetName());
            }
        }
    }
}

bool AWorldItem::MainInteract(AFrontierDiverCharacter* Character)
{
    if (UInventoryComponent* InventoryComp = Character->FindComponentByClass<UInventoryComponent>())
    {
        return InventoryComp->PickupItemToInventory(this);
    }
    return false;
}

void AWorldItem::LoadDataToWorldItem(TStrongObjectPtr<UItemDynamicInfo> ItemDynamic, const TSharedPtr<FItemTableRowInfoBase> ItemTableRowInfo, TSubclassOf<UItemBase> NewItemType)
{
    ItemType = NewItemType;
    ItemDynamicInfo = MoveTemp(ItemDynamic);

    if (StaticMesh && ItemTableRowInfo.IsValid())
    {
        StaticMesh->SetStaticMesh(ItemTableRowInfo->WorldItemStaticMesh);
        StaticMesh->SetWorldScale3D(ItemTableRowInfo->WorldItemScale);
        if (FoundActor.IsValid())
        {
            AInventoryDataTableItemManager* ValidFoundActor = FoundActor.Get();
            if (ValidFoundActor)
            {
                if (ValidFoundActor->WaterPhysicalMaterial)
                {
                    StaticMesh->SetPhysMaterialOverride(ValidFoundActor->WaterPhysicalMaterial);
                }
            }
        }
        StaticMesh->SetMassOverrideInKg(NAME_None, ItemTableRowInfo->WorldItemMass);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("StaticMesh or ItemTableRowInfo is null."));
    }
}