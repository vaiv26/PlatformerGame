// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatCharacterEnemy.h"

#include "AIController.h"
#include "PlatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlatCharacterEnemy::APlatCharacterEnemy()
{
	// Set movement speed
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(1, 0, 0)); // Lock X movement
}

void APlatCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentTarget = PointB;
}

void APlatCharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!CurrentTarget) return;

	FVector MyLoc = GetActorLocation();
	FVector TargetLoc = CurrentTarget->GetActorLocation();

	// Lock movement to Y axis only (2.5D)
	TargetLoc.X = MyLoc.X;
	TargetLoc.Z = MyLoc.Z;

	float Distance = FVector::Dist2D(MyLoc, TargetLoc);

	// Move towards target manually
	if (Distance > AcceptanceRadius)
	{
		FVector Direction = (TargetLoc - MyLoc).GetSafeNormal2D();
		AddMovementInput(Direction, 1.0f);

		// Flip the mesh (not the actor)
		if (Direction.Y != 0)
		{
			FVector Scale = GetActorScale3D();
			float DesiredSign = FMath::Sign(Direction.Y);

			if (FMath::Sign(Scale.Y) != DesiredSign)
			{
				Scale.Y = DesiredSign * FMath::Abs(Scale.Y);
				SetActorScale3D(Scale);
			}
		}
	}
	else
	{
		// Switch target
		CurrentTarget = (CurrentTarget == PointA) ? PointB : PointA;
	}

}

void APlatCharacterEnemy::Die()
{
	Super::Die();
}

void APlatCharacterEnemy::PatrolUpdate()
{
}
