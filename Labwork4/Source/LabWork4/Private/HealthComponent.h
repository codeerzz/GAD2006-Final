// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	UPROPERTY(EditAnywhere)
	int Health; // Karakterin sa?l?k puan?


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:


	bool bCanTakeDamage; // Hasar alabilir mi?

	void Die(); // Ölüm i?levi
	void AllowTakeDamage(); // Ölüm i?levi

public:
	void TakeDamage(int Damage); // Hasar alma i?levi



public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
