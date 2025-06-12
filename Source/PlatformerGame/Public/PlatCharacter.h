// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatCharacterBase.h"
#include "PlatCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class PLATFORMERGAME_API APlatCharacter : public APlatCharacterBase
{
	GENERATED_BODY()

public:
	APlatCharacter();

	// Combat Functions
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttack() const;

	// Override from CharacterBase
	virtual void TakeDamage(float DamageAmount) override;
	virtual void Die() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	// Sword Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UStaticMeshComponent> SwordComponent;

	// Combat Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 150.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCanAttack = true;

	// Invulnerability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float InvulnerabilityTime = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsInvulnerable = false;

	UPROPERTY()
	FTimerHandle InvulnerabilityTimerHandle;
	UPROPERTY()
	FTimerHandle AttackCooldownTimerHandle;
	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

private:
	void EndAttack();
	void EndInvulnerability();
	void ConstrainToYAxis();
};
