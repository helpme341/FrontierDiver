// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class FRONTIERDIVER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	TStrongObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WidgetAirTextBlock;

	void UpdateWidgetForAir();

	class TStrongObjectPtr<AFrontierDiverCharacter> DiverCharacter;
};
