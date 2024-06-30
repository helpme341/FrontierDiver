// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FrontierDiverCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/Inventory/InventoryComponent.h"
#include <Components/Inventory/Items/Items/JewelryItem.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
AFrontierDiverCharacter::AFrontierDiverCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FrontierDiverCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FrontierDiverCameraComponent->SetupAttachment(GetCapsuleComponent());
	FrontierDiverCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FrontierDiverCameraComponent->bUsePawnControlRotation = true;

	FrontierDiverInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FrontierDiverCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


}

// Called when the game starts or when spawned
void AFrontierDiverCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
 	}
	UJewelryItem* Test = NewObject<UJewelryItem>();
	Test->ItemDynamicInfo.ItemTypeName = "1";
	if (FrontierDiverInventoryComponent->AddItemToInventory(Test))
	{
		Test->ItemDynamicInfo.ItemTypeName = "2";
	}
}

void AFrontierDiverCharacter::Tick(float DeltaTime)
{
}

void AFrontierDiverCharacter::ChangeMovementMode(EMovementMode NewMovementMode)
{
	switch (NewMovementMode)
	{
	case MOVE_Walking:
		GetCharacterMovement()->SetMovementMode(NewMovementMode);
		if (bIsSwimming)
		{
			GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
			GetCharacterMovement()->GravityScale = 1.0;
			bIsSwimming = false;
		}
		break;
	case MOVE_Swimming:
		if (!bIsSwimming) {
			GetCharacterMovement()->SetMovementMode(NewMovementMode);
			GetCharacterMovement()->GravityScale = 0.15;
			GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
			bIsSwimming = true; 
		}
		break;
	default:
		break;
	}
}

void AFrontierDiverCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!bIsSwimming) { AddMovementInput(GetActorForwardVector(), MovementVector.Y); }
	else { AddMovementInput(GetControlRotation().Vector(), MovementVector.Y); }
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AFrontierDiverCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFrontierDiverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::Jump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrontierDiverCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AFrontierDiverCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrontierDiverCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}