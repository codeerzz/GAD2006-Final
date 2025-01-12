// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ATrap::ATrap()
{
    OnActorBeginOverlap.AddDynamic(this, &ATrap::OnOverlapBegin);
}

void ATrap::BeginPlay()
{
    Super::BeginPlay();
}

void ATrap::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    // Eğer dokunan bir karakterse
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character)
    {
        // Karakterin hareket hızını sıfırla
        Character->GetCharacterMovement()->MaxWalkSpeed = 0.0f;

        // 3 saniye sonra hareket hızını eski haline döndür
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ATrap::ResetCharacterSpeed, Character), 3.0f, false);
    }
}

void ATrap::ResetCharacterSpeed(ACharacter* Character)
{
    if (Character)
    {
        Character->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    }
}
