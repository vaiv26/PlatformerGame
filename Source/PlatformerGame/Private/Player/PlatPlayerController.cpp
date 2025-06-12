// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlatPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlatCharacter.h"
#include "GameFramework/Character.h"

APlatPlayerController::APlatPlayerController()
{
}

void APlatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APlatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Cast to Enhanced Input Component
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind Move Action
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlatPlayerController::Move);

		// Bind Jump Actions
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APlatPlayerController::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlatPlayerController::StopJumping);

		// Bind Attack Action
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APlatPlayerController::Attack);
	}
}

void APlatPlayerController::Move(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();
   
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementValue);
    
		// Flip mesh to face movement direction
		if (MovementValue != 0.0f)
		{
			if (USkeletalMeshComponent* MeshComp = ControlledPawn->FindComponentByClass<USkeletalMeshComponent>())
			{
				FVector MeshScale = MeshComp->GetRelativeScale3D();
				MeshScale.Y = (MovementValue > 0.0f) ? 1.0f : -1.0f;
				MeshComp->SetRelativeScale3D(MeshScale);
			}
		}
	}
}

void APlatPlayerController::Jump()
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		PlayerCharacter->Jump();
	}
}

void APlatPlayerController::StopJumping()
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		PlayerCharacter->StopJumping();
	}
}

void APlatPlayerController::Attack()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		UE_LOG(LogTemp,Warning,TEXT("Command is here in side Player Controller Attack first if"));
		if (APlatCharacter* PlayerCharacter = Cast<APlatCharacter>(ControlledPawn))
		{
			UE_LOG(LogTemp,Warning,TEXT("Command is here in side PlayerController Attack Second if"));
			PlayerCharacter->Attack();
		}
	}
}
