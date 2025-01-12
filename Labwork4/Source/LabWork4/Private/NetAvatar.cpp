// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"
#include "HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANetAvatar::ANetAvatar() :
	RunningSpeed(600.0f),
	WalkingSpeed(300.0f),
	bIsRunning(false),
	MovementScale(1.0f),
	Damage(100.0f),
	Health(100.0f)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}


void ANetAvatar::BeginPlay()
{
	Super::BeginPlay();

	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ANetAvatar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetAvatar, bIsRunning);
	DOREPLIFETIME(ANetAvatar, bIsAttacking);
}

void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ANetAvatar::StartRunning);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ANetAvatar::StopRunning);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ANetAvatar::Attack);
	PlayerInputComponent->BindAction("HeavyAttack", EInputEvent::IE_Pressed, this, &ANetAvatar::UsePunch);
}

void ANetAvatar::MoveForward(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementScale * Scale);

}

void ANetAvatar::MoveRight(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, MovementScale * Scale);
}

void ANetAvatar::StartRunning()
{
	if (HasAuthority())
	{
		bIsRunning = true;
		OnRep_IsRunning();
	}
	else
	{
		ServerStartRunning();
	}
}

void ANetAvatar::StopRunning()
{
	if (HasAuthority())
	{
		bIsRunning = false;
		OnRep_IsRunning();
	}
	else
	{
		ServerStopRunning();
	}
}

void ANetAvatar::ServerStartRunning_Implementation()
{
	StartRunning();
}

void ANetAvatar::ServerStopRunning_Implementation()
{
	StopRunning();
}

void ANetAvatar::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
}

void ANetAvatar::PerformAttack()
{
	// Saldırı animasyonunu başlat
	if (AttackAnimation && !bIsAttacking)
	{
		GetMesh()->PlayAnimation(AttackAnimation, false);
	
	}


}

void ANetAvatar::Attack()
{
	if (HasAuthority())
	{
		PerformAttack();
		bIsAttacking = true;
		
	}
	else
	{
		ServerAttack();
	}
}

void ANetAvatar::LineTrace()
{


	FVector StartLocation = SwordMesh->GetSocketLocation(FName("Start"));
	FVector EndLocation = SwordMesh->GetSocketLocation(FName("End"));

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);

	if (HitResult.bBlockingHit)
	{
		AActor* ActorHit = HitResult.GetActor();
		UHealthComponent* EnemyHit = ActorHit->FindComponentByClass<UHealthComponent>();
		if (IsValid(ActorHit))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *ActorHit->GetName());
		}

		if (IsValid(EnemyHit))
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Component Found: %s"), *EnemyHit->GetOwner()->GetName());
			EnemyHit->TakeDamage(Damage); // Sunucuda hasar uygula
		}

	}
}

void ANetAvatar::ServerAttack_Implementation()
{
	bIsAttacking = true;
	OnRep_IsAttacking(); // İstemcilere animasyon durumunu bildir
}

void ANetAvatar::OnRep_IsAttacking()
{
	if (bIsAttacking && AttackAnimation)
	{
		UE_LOG(LogTemp, Log, TEXT("Playing attack animation on client."));
		GetMesh()->PlayAnimation(AttackAnimation, false);
	}
}



void ANetAvatar::UsePunch()
{
	if (bCanUsePunch)
	{
		PlayPunchAnimation();

		FVector ForwardVector = GetActorForwardVector(); 
		FVector TeleportLocation = GetActorLocation() + ForwardVector * 500.0f; 

		FHitResult HitResult;
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + ForwardVector * 500.0f;

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				
				HitActor->SetActorLocation(TeleportLocation);

				UE_LOG(LogTemp, Warning, TEXT("%s teleported to %s"), *HitActor->GetName(), *TeleportLocation.ToString());
			}
		}

		bCanUsePunch = false;
		GetWorld()->GetTimerManager().SetTimer(PunchCooldownTimerHandle, this, &ANetAvatar::ResetPunchCooldown, 1.0f, false); // 10 saniye cooldown
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Punch is on cooldown!"));
	}
}

void ANetAvatar::PlayPunchAnimation()
{
	if (Punch_Animation1)
	{
		GetMesh()->PlayAnimation(Punch_Animation1, false);
	}
}

void ANetAvatar::ResetPunchCooldown()
{
	bCanUsePunch = true;
}