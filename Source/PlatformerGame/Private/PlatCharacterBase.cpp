// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlatCharacterBase::APlatCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// Set default health
	CurrentHealth = MaxHealth;

}

// Called when the game starts or when spawned
void APlatCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlatCharacterBase::TakeDamage(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerBase::TakeDamage"));
	if (!IsAlive()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Enemy Health: %.0f"), CurrentHealth));
	
	if (CurrentHealth <= 0.0f)
	{
		Die();
	}
}

void APlatCharacterBase::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        
	// Disable character movement
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	Destroy();
}


