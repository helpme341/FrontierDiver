// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FrontierDiverGameMode.h"

AFrontierDiverGameMode::AFrontierDiverGameMode()
    : Super()
{
		// set default pawn class to our Blueprinted character
		static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
		DefaultPawnClass = PlayerPawnClassFinder.Class;
}
