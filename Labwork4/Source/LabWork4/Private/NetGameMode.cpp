// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameMode.h"
#include "NetBaseCharacter.h"
#include "Blueprint/UserWidget.h"


ANetGameMode::ANetGameMode()
{
	DefaultPawnClass = ANetBaseCharacter::StaticClass();

    VictoryScore =  5;
    CurrentScore = 0;

}

void ANetGameMode::AddScore(int32 Score)
{
    CurrentScore += Score;
    UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), CurrentScore);

    if (CurrentScore >= VictoryScore)
    {
        TriggerVictory();
    }
}

void ANetGameMode::TriggerVictory()
{
    UE_LOG(LogTemp, Warning, TEXT("Victory! Player has reached the required score."));
    if (VictoryWidgetClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            UUserWidget* VictoryWidget = CreateWidget<UUserWidget>(PlayerController, VictoryWidgetClass);
            if (VictoryWidget)
            {
                VictoryWidget->AddToViewport();
                PlayerController->SetShowMouseCursor(true); // Fare imlecini görünür yap
                PlayerController->SetInputMode(FInputModeUIOnly()); // Sadece UI ile etkileşim sağla
            }
        }
    }
    // Oyunculara zafer mesajı gönderin
  
    // Oyun sonu işlemleri
 
}

void ANetGameMode::PlayerLost()
{
    UE_LOG(LogTemp, Warning, TEXT("Player has lost the game!"));
    if (LoseWidgetClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            UUserWidget* LoseWidget = CreateWidget<UUserWidget>(PlayerController, LoseWidgetClass);
            if (LoseWidget)
            {
                LoseWidget->AddToViewport();
                PlayerController->SetShowMouseCursor(true); // Fare imlecini görünür yap
                PlayerController->SetInputMode(FInputModeUIOnly());
            }
        }
    }



}
