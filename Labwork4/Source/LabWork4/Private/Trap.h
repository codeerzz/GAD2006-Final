// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Character.h"
#include "Trap.generated.h"

/**
 * 
 */
UCLASS()
class ATrap : public ATriggerBox
{
	GENERATED_BODY()


    protected:
        virtual void BeginPlay() override;

    public:


        ATrap();

        UFUNCTION()
        void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

        UFUNCTION()
        void ResetCharacterSpeed(ACharacter* Character);




	
};
