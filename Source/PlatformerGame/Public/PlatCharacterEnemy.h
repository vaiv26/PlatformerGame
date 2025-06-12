// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatCharacterBase.h"
#include "PlatCharacterEnemy.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PLATFORMERGAME_API APlatCharacterEnemy : public APlatCharacterBase
{
	GENERATED_BODY()

public:
	APlatCharacterEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, Category="2.5D Movement")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category="2.5D Movement")
	TObjectPtr<AActor> PointA;

	UPROPERTY(EditAnywhere, Category="2.5D Movement")
	TObjectPtr<AActor> PointB;

	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptanceRadius = 10.0f;


public:
	virtual void Die() override;

private:
	void PatrolUpdate();
	
};
