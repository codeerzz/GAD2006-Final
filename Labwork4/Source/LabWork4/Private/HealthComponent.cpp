// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "NetGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCanTakeDamage = true;
	Health = 200;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UHealthComponent::Die()
{
	ANetGameMode* GameMode = Cast<ANetGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), GameMode->CurrentScore);
		GameMode->AddScore(1); // Örneğin, her düşman 10 puan kazandırır.
	}


	GetOwner()->Destroy();
}

void UHealthComponent::AllowTakeDamage()
{
	bCanTakeDamage = true; // Hasar almayı tekrar etkinleştir
}

void UHealthComponent::TakeDamage(int Damage)
{
	
		if (bCanTakeDamage)
		{
			Health -= Damage;
			bCanTakeDamage = false;
			FTimerHandle InvincibilityDelay;
			GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UHealthComponent::AllowTakeDamage, 0.5f, false);

			if (Health <= 0)
			{
				Die();
			}
		}

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

