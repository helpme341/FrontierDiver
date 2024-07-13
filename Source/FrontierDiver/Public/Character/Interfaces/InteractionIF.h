// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionIF.generated.h"

class AFrontierDiverCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionIF : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTIERDIVER_API IInteractionIF : public IInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool MainInteract(AFrontierDiverCharacter* Character) { return false; };

	virtual bool FirstInteract(AFrontierDiverCharacter* Character) { return false; };

	virtual bool SecondInteract(AFrontierDiverCharacter* Character) { return false; };

	virtual bool ThirdInteract(AFrontierDiverCharacter* Character) { return false; };
};
