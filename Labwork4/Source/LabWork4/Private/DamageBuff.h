// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "NetAvatar.h" 
#include "DamageBuff.generated.h"

/**
 * 
 */
UCLASS()
class ADamageBuff : public ATriggerBox
{
	GENERATED_BODY()
	

    protected:
	    virtual void BeginPlay() override;

    public:
        ADamageBuff();

        UFUNCTION()
        void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

    private:
        void ResetDamage(ANetAvatar* Character, int OriginalDamage);











};
