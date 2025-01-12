// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NetAvatar.generated.h"

/**
 * 
 */
UCLASS()
class ANetAvatar : public ANetBaseCharacter
{
	GENERATED_BODY()
	
public:

	ANetAvatar();

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite)
	float MovementScale;

	UPROPERTY(BlueprintReadWrite)
	float Health;

	UPROPERTY(BlueprintReadWrite)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsRunning)
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsRunning)
	bool bCanUsePunch = true; // Punch kullanılabilirliği


	UPROPERTY(BlueprintReadWrite)
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* PunchAnimation; // kilic animasyonu fakat cok oynadim karisti bir tik


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimationAsset* Punch_Animation1;





	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);


	UFUNCTION()
	void StartRunning();

	UFUNCTION()
	void StopRunning();

	UPROPERTY()
	float RunningSpeed;



	UFUNCTION(Server, Reliable)
	void ServerStartRunning();

	UFUNCTION(Server, Reliable)
	void ServerStopRunning();

	UFUNCTION(Server, Reliable)
	void ServerAttack();

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsRunning)
	bool bIsRunning;


	UFUNCTION()
	void UsePunch();

	UFUNCTION()
	void PlayPunchAnimation();

	void ResetPunchCooldown();

	FTimerHandle PunchCooldownTimerHandle;

	
	UFUNCTION()
	void OnRep_IsRunning();

	UFUNCTION()
	void PerformAttack();

	UFUNCTION()
	void Attack();

	UFUNCTION(BlueprintCallable)
	void LineTrace();

	UFUNCTION()
	void OnRep_IsAttacking();

};
