// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBuff.h"
#include "NetAvatar.h"
#include "TimerManager.h"


ADamageBuff::ADamageBuff()
{
    OnActorBeginOverlap.AddDynamic(this, &ADamageBuff::OnOverlapBegin);
}


void ADamageBuff::BeginPlay()
{
    Super::BeginPlay();
}

void ADamageBuff::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    // Eğer tetikleyiciye giren bir NetAvatar ise
    ANetAvatar* Character = Cast<ANetAvatar>(OtherActor);
    if (Character)
    {
        // Orijinal Damage değerini sakla
        int OriginalDamage = Character->Damage;

        // Damage değerini 2 katına çıkar
        Character->Damage *= 2;
        UE_LOG(LogTemp, Warning, TEXT("Character %s entered the DamageBuffTrigger."), *Character->GetName());
        // 5 saniye sonra eski Damage değerine geri dön
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ADamageBuff::ResetDamage, Character, OriginalDamage), 10.0f, false);
    }
}

void ADamageBuff::ResetDamage(ANetAvatar* Character, int OriginalDamage)
{
    if (Character)
    {
        Character->Damage = OriginalDamage; // Damage değerini eski haline getir
    }
}