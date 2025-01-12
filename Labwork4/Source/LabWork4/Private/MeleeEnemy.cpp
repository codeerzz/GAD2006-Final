// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"
#include "HealthComponent.h"
#include "NetAvatar.h"
#include "NetGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMeleeEnemy::AMeleeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);

	
}

// Called when the game starts or when spawned
void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AMeleeEnemy::ApplyDamageToPlayer()
{
	if (PlayerPawn)
	{
		ANetAvatar* Player = Cast<ANetAvatar>(PlayerPawn);
		if (Player)
		{
			Player->Health -= 10.0f; 
			UE_LOG(LogTemp, Warning, TEXT("Player Health: %f"), Player->Health);

			// Oyuncunun sağlığı sıfıra ulaşırsa
			if (Player->Health <= 0.0f)
			{
				Player->Health = 0.0f; 
				ANetGameMode* GameMode = Cast<ANetGameMode>(GetWorld()->GetAuthGameMode());
				if (GameMode)
				{
					GameMode->PlayerLost();
				}

				UE_LOG(LogTemp, Warning, TEXT("Player is dead!"));
			}
		}
	}



}

// Called every frame
void AMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPlayerInChaseRange())
	{
		ChasePlayer(DeltaTime);
		RotateTowardsPlayer(); // Oyuncuya dön


	}
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (DistanceToPlayer <= 100)
	{

		if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AMeleeEnemy::ApplyDamageToPlayer, 0.3f, true);
		}
		UE_LOG(LogTemp, Warning, TEXT("dibinde"));
		PlayAttackAnimation();




	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle); // Oyuncu uzaklaşırsa timer'ı durdur
	}
}



void AMeleeEnemy::ChasePlayer(float DeltaTime)
{
	// Oyuncuya doğru yön vektörünü hesapla
	FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	// Oyuncuya olan mesafeyi kontrol et
	if (FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) > AcceptanceRadius)
	{
		// Yürümek için hareket girişi ekle
		AddMovementInput(Direction);
	}
}

void AMeleeEnemy::PlayAttackAnimation()
{
	if (AttackAnimation && GetMesh()->GetAnimInstance())
	{
		GetMesh()->PlayAnimation(AttackAnimation, false);
	}

}

bool AMeleeEnemy::IsPlayerInChaseRange() const
{
	if (!PlayerPawn) return false;

	float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	return DistanceToPlayer <= ChaseRadius;
}

void AMeleeEnemy::RotateTowardsPlayer()
{
	if (!PlayerPawn) return;

	// AI karakter ile oyuncu arasındaki yön vektörünü al
	FVector DirectionToPlayer = (PlayerPawn->GetActorLocation() - GetActorLocation());
	DirectionToPlayer.Z = 0.0f; // Z eksenini sıfırla, sadece yatay düzlemde döndür

	if (!DirectionToPlayer.IsNearlyZero())
	{
		FRotator TargetRotation = DirectionToPlayer.ToOrientationRotator();

		// 90 derece ofset ekle
		TargetRotation.Yaw -= 90.0f;

		// Yumuşak dönüş için interpolasyon
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f));
	}
}

