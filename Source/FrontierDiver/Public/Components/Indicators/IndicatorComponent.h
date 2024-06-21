// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Base/CharacterComponentBase.h"
#include "IndicatorComponent.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UIndicatorComponent : public UCharacterComponentBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0.0f;

	void DamageHealth(float Amount) { Health = FMath::Clamp(Health - Amount, 0.0f, MaxHealth); };

	void AddHealth(float Amount) { Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth); };

};
