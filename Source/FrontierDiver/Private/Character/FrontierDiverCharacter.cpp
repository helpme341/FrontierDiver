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
#include "DrawDebugHelpers.h" 
#include "Widgets/MainWidget.h"
#include "Components/Inventory/Items/ItemsTypes/BreathingTankItem.h"

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

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void AFrontierDiverCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Swimming);
	GetCharacterMovement()->GravityScale = 0.15;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		MainWidget = CreateWidget<UMainWidget>(PlayerController, MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}

		MainWidget->DiverCharacter.Reset(this);
		InventoryComponent->BeginLogic(this);
		MainWidget->InventoryWidget = InventoryComponent->InventoryWidget;
		MainWidget->UpdateWidgetForAir();
	}
}

void AFrontierDiverCharacter::UseAir(float AirAmount)
{
	if (AirAmount <= 0) { return; }
	if (InventoryComponent->bIsBreathingTankItemHeld && InventoryComponent->HeldBreathingTankItem->GetBreathingTankAir() > 0)
	{
		InventoryComponent->HeldBreathingTankItem->SetBreathingTankAir(-AirAmount);
		MainWidget->UpdateWidgetForAir();
	}
	else if (CurrentAir > 0)
	{
		CurrentAir = FMath::Clamp(CurrentAir - AirAmount, 0.0f, MaxAir);
		MainWidget->UpdateWidgetForAir();
	}

	if (CurrentAir <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of air!"));///////
	}
}

void AFrontierDiverCharacter::ReplenishAir(float AirAmount)
{
	if (CurrentAir < MaxAir)
	{
		CurrentAir = FMath::Clamp(CurrentAir + AirAmount, 0.0f, MaxAir);
		MainWidget->UpdateWidgetForAir();
	}
	else if (InventoryComponent->bIsBreathingTankItemHeld && InventoryComponent->HeldBreathingTankItem->GetBreathingTankAir() < InventoryComponent->HeldBreathingTankItem->GetBreathingTankMaxAir())
	{
		InventoryComponent->HeldBreathingTankItem->SetBreathingTankAir(AirAmount);
		MainWidget->UpdateWidgetForAir();
	}
}

float AFrontierDiverCharacter::GetGlobalCurrentAir()
{
	if (InventoryComponent->bIsBreathingTankItemHeld)
	{
		return CurrentAir + InventoryComponent->HeldBreathingTankItem->GetBreathingTankAir();
	}
	return CurrentAir;
}

float AFrontierDiverCharacter::GetGlobalMaxAir()
{
	if (InventoryComponent->bIsBreathingTankItemHeld)
	{
		return MaxAir + InventoryComponent->HeldBreathingTankItem->GetBreathingTankMaxAir();
	}
	return MaxAir;
}

void AFrontierDiverCharacter::StartUsingAir()
{
	if (!bIsAirUsing)
	{
		AirUsageTimerDel = FTimerDelegate::CreateLambda([this]()
			{
				this->UseAir(AirConsumptionRate);
			});
		GetWorld()->GetTimerManager().SetTimer(AirUsageTimerHandle, AirUsageTimerDel, AirRefreshRate, true);
		bIsAirUsing = true;
	}
}

void AFrontierDiverCharacter::StopUsingAir()
{
	if (bIsAirUsing)
	{
		GetWorld()->GetTimerManager().ClearTimer(AirUsageTimerHandle);
		bIsAirUsing = false;
	}
}

void AFrontierDiverCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	//if (!bIsSwimming) { AddMovementInput(GetActorForwardVector(), MovementVector.Y); }
	//else { AddMovementInput(GetControlRotation().Vector(), MovementVector.Y); }

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AFrontierDiverCharacter::Look(const FInputActionValue& Value)
{
	if (InventoryComponent->InventoryWidget->InventoryIsHidden())
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

void AFrontierDiverCharacter::MainInteract()
{
	FVector StartLocation = GetActorLocation();
	StartLocation.Z += 60;
	FVector EndLocation = StartLocation + GetControlRotation().Vector() * InteractDistance;
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, GetOwner());
	FHitResult HitResult;

	float SphereRadius = 25.0f;

	// Визуализируем линию трассировки
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 2.0f, 0, 1.0f);
	// Визуализируем сферическую трассировку в начале и в конце
	DrawDebugSphere(GetWorld(), StartLocation, SphereRadius, 12, FColor::Blue, false, 2.0f);
	DrawDebugSphere(GetWorld(), EndLocation, SphereRadius, 12, FColor::Red, false, 2.0f);

	if (GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), TraceParams))
	{
		if (IInteractionIF* InteractionIF = Cast<IInteractionIF>(HitResult.GetActor()))
		{
			InteractionIF->MainInteract(this);
		}

		// Визуализируем точку удара
		DrawDebugSphere(GetWorld(), HitResult.Location, SphereRadius, 12, FColor::Yellow, false, 2.0f);
	}
	// ещё логика
}

void AFrontierDiverCharacter::FirstInteract()
{
	if (InventoryComponent->FirstInteractWithHeldItem()) { return; }
	// ещё логика
}

void AFrontierDiverCharacter::SecondInteract()
{
	if (InventoryComponent->SecondInteractWithHeldItem()) { return; }
	// ещё логика
}

void AFrontierDiverCharacter::ThirdInteract()
{
	if (InventoryComponent->ThirdInteractWithHeldItem()) { return; }
	// ещё логика
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

		// Lookingeeee
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrontierDiverCharacter::Look);

		//Interact
		EnhancedInputComponent->BindAction(MainInteractAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::MainInteract);
		
		EnhancedInputComponent->BindAction(FirstInteractAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::FirstInteract);

		EnhancedInputComponent->BindAction(SecondInteractAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::SecondInteract);

		EnhancedInputComponent->BindAction(ThirdInteractAction, ETriggerEvent::Started, this, &AFrontierDiverCharacter::ThirdInteract);



		//Inventory Interact
		EnhancedInputComponent->BindAction(OpenCloseInventoryAction, ETriggerEvent::Started, InventoryComponent, &UInventoryComponent::OpenCloseInventory);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}