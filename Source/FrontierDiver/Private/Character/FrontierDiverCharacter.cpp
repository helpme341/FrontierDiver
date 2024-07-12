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
#include "Character/Interfaces/InteractionIF.h"
#include "Components/Inventory/Widgets/InventoryWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
AFrontierDiverCharacter::AFrontierDiverCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);


	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Create a CameraComponent	
	FrontierDiverCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FrontierDiverCameraComponent->SetupAttachment(Mesh1P,TEXT("head"));
	FrontierDiverCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FrontierDiverCameraComponent->bUsePawnControlRotation = true;

	FrontierDiverInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

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
	if (FrontierDiverInventoryComponent->InventoryWidget->InventoryIsHidden())
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
}

void AFrontierDiverCharacter::Interact()
{
	FVector StartLocation = GetActorLocation();
	StartLocation.Z += 60;
	FVector EndLocation = StartLocation + GetControlRotation().Vector() * InteractDistance;
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, GetOwner());
	FHitResult HitResult;

	float SphereRadius = 25.0f;

	if (GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(15.0f), TraceParams))
	{
		if (IInteractionIF* InteractionIF = Cast<IInteractionIF>(HitResult.GetActor())) { InteractionIF->Interaction(this); }
	}
}

void AFrontierDiverCharacter::InventoryInteract()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		if (FrontierDiverInventoryComponent->InventoryWidget->InventoryIsHidden())
		{
			// Открытие инвентаря
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			PlayerController->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(FrontierDiverInventoryComponent->InventoryWidget->TakeWidget()).SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock));
			FrontierDiverInventoryComponent->InventoryWidget->SetNotQuickInventoryVisibility(false);
		}
		else
		{
			// Закрытие инвентаря
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
			FrontierDiverInventoryComponent->InventoryWidget->SetNotQuickInventoryVisibility(true);
		}
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

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::Interact);


		//Inventory Interact
		EnhancedInputComponent->BindAction(InventoryInteractAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::InventoryInteract);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}