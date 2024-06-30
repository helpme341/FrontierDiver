// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/Items/WorldItems/WorldItemBase.h"

// Sets default values
AWorldItemBase::AWorldItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

