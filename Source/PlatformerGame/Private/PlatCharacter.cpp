// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatCharacter.h"

#include "PlatCharacterEnemy.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlatCharacter::APlatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 600.0f, 50.0f));

	// Create Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Create Sword Component
	SwordComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordComponent->SetupAttachment(GetMesh(), TEXT("SwordR"));

	// Set Character Movement
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->GravityScale = 1.5f;
}

void APlatCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	    
	// Constrain movement to Y-axis for 2.5D
	ConstrainToYAxis();
}

void APlatCharacter::Attack()
{
	if (!bCanAttack || bIsAttacking || !IsAlive()) return;
	
	bIsAttacking = true;
	bCanAttack = false;

	// Simple sword rotation animation would go here
	// For now, just perform the attack after a short delay
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APlatCharacter::PerformAttack, 0.15f, false);
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &APlatCharacter::EndAttack, 1.0f, false);

}

void APlatCharacter::PerformAttack() const
{
	if (AttackMontage && GetMesh())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
		
		FVector StartLocation = GetActorLocation();

		// Get correct attack direction based on mesh flip
		USkeletalMeshComponent* MeshComp = GetMesh();
		float MeshScaleY = MeshComp->GetRelativeScale3D().Y;
		FVector AttackDirection = FVector(0.0f, MeshScaleY, 0.0f); // Uses mesh scale sign

		FVector EndLocation = StartLocation + (AttackDirection * AttackRange);

		// Sphere trace for attack
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FHitResult HitResult;
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeSphere(50.0f),
			QueryParams
		);

		if (bHit)
		{
			if (APlatCharacterEnemy* Enemy = Cast<APlatCharacterEnemy>(HitResult.GetActor()))
			{
				Enemy->TakeDamage(AttackDamage);
			}
		}
		
		// Debug visualization
		//DrawDebugSphere(GetWorld(), EndLocation, 50.0f, 12, FColor::Red, false, 1.0f);
	}
}

void APlatCharacter::TakeDamage(float DamageAmount)
{
	if (bIsInvulnerable || !IsAlive()) return;
	Super::TakeDamage(DamageAmount);

	bIsInvulnerable = true;
	GetWorld()->GetTimerManager().SetTimer(InvulnerabilityTimerHandle, this, &APlatCharacter::EndInvulnerability, InvulnerabilityTime, false);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player Health: %.0f"), CurrentHealth));

}

void APlatCharacter::Die()
{
	Super::Die();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GAME OVER!"));
    
	// Disable input and hide player
	SetActorHiddenInGame(true);
	DisableInput(Cast<APlayerController>(GetController()));
}

void APlatCharacter::EndAttack()
{
	bIsAttacking = false;
	bCanAttack = true;
}

void APlatCharacter::EndInvulnerability()
{
	bIsInvulnerable = false;
}

void APlatCharacter::ConstrainToYAxis()
{
	FVector CurrentLocation = GetActorLocation();
	SetActorLocation(FVector(0.0f, CurrentLocation.Y, CurrentLocation.Z));
}
