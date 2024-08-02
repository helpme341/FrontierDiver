// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/ItemBase.h"
#include "Components/Inventory/Items/WorldItem.h"
#include "Components/Inventory/Widgets/InventoryItemWidget.h"
#include "Components/Inventory/InventoryComponent.h"

void UItemBase::PlayItemAnimMontage(UAnimMontage* MontageToPlay, FName& AnimSlot, UInventoryComponent* Inventory)
{
	if (Inventory && MontageToPlay && Inventory->PlayerAnimInstance.IsValid())
	{
		Inventory->PlayerAnimInstance->Montage_Play(MontageToPlay);
		if (AnimSlot != "None") { Inventory->PlayerAnimInstance->Montage_JumpToSection(FName("Default"), MontageToPlay); }
	}
}
