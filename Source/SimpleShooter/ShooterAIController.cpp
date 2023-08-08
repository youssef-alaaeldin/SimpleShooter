// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior)
    {
        RunBehaviorTree(AIBehavior);
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }

}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
   


    bool bIsInSight = LineOfSightTo(PlayerPawn); 
    if (bIsInSight)
    {
        // Setting PlayerLocation
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        // Setting LastKnown
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownLocation"), PlayerPawn->GetActorLocation());
    }
    else
    {
        // Clear PlayerLocation
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }
}
