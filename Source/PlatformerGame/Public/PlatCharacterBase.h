// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlatCharacterBase.generated.h"

UCLASS(Abstract)
class PLATFORMERGAME_API APlatCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlatCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	// Health System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

public:	
	
	// Health Functions
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void Die();

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

};
