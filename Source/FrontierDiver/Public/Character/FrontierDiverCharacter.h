// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FrontierDiverCharacter.generated.h"

UENUM(BlueprintType)
enum class EAnimItemBlendType : uint8
{
	None,
	Test,
};


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInventoryComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class FRONTIERDIVER_API AFrontierDiverCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFrontierDiverCharacter();

private:

	/** Inventory Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MainInteractAction;

	/** First Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FirstInteractAction;

	/** Second Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondInteractAction;

	/** Second Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThirdInteractAction;

	/** Inventory Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenCloseInventoryAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float InteractDistance = 75.0f;

public:

	// Variables for air management
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Air Management")
	float CurrentAir = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Air Management")
	float CurrentMaxAir = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Air Management")
	float AirRefreshRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Air Management")
	float AirConsumptionRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Air Management")
	float MaxAir = 1.0f;



	// Methods for air management
	UFUNCTION(BlueprintCallable, Category = "Air Management")
	void UseAir(float AirAmount);

	UFUNCTION(BlueprintCallable, Category = "Air Management")
	void ReplenishAir(float AirAmount);

	UFUNCTION(BlueprintCallable, Category = "Air Management")
	void IncreaseMaxAir(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Air Management")
	void DecreaseMaxAir(float Amount);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Air Management")
	bool bIsAirUsing;

	UFUNCTION(BlueprintCallable, Category = "Air Management")
	void StartUsingAir();
	UFUNCTION(BlueprintCallable, Category = "Air Management")
	void StopUsingAir();

	FTimerHandle AirUsageTimerHandle;

	FTimerDelegate AirUsageTimerDel;

public:

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FrontierDiverCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings")
	EAnimItemBlendType  AnimItemBlendTypeNow;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void MainInteract();
	     
	void FirstInteract();

	void SecondInteract();

	void ThirdInteract();

	void OpenCloseInventory();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FrontierDiverCameraComponent; }
};
