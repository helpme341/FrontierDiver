// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterComponentBase.generated.h"

class AFrontierDiverCharacter;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRONTIERDIVER_API UCharacterComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterComponentBase();

	void CastToOwners();

	AFrontierDiverCharacter* OwnerCharacter;

	UCharacterMovementComponent* OwnerCharacterMovementComponent;
};
