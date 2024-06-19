// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Base/CharacterComponentBase.h"
#include "Character/FrontierDiverCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UCharacterComponentBase::UCharacterComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCharacterComponentBase::CastToOwners()
{
	OwnerCharacter = Cast<AFrontierDiverCharacter>(GetOwner());
	OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
}
