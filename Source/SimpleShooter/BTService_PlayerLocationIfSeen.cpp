// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld() , 0);
    
    if(PlayerPawn == nullptr)
    {
        return;
    }

    if (AIController->LineOfSightTo(PlayerPawn))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }


}
