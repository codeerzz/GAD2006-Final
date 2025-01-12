// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.h" // HealthComponent'i ekledik
#include "MeleeEnemy.generated.h"

UCLASS()
class AMeleeEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMeleeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle AttackTimerHandle;

	void ApplyDamageToPlayer(); // Hasar verme işle

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "AI")
	float ChaseRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptanceRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 200.0f;  // AI'nin hareket hızı

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent; // Sağlık bileşeni referansı

	UPROPERTY(EditAnywhere)
	class UAnimSequence* AttackAnimation;

private:

	APawn* PlayerPawn;

	UFUNCTION()
	void ChasePlayer(float DeltaTime);

	UFUNCTION()
	void PlayAttackAnimation();

	UFUNCTION()
	bool IsPlayerInChaseRange() const;

	void RotateTowardsPlayer();



};
