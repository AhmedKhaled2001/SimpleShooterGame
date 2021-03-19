// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"

void AEnemyCharacterController::BeginPlay() 
{
    Super::BeginPlay();
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }

}
void AEnemyCharacterController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
    
    GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), GetPawn()->GetActorLocation());
}

bool AEnemyCharacterController::IsDead() 
{
    AEnemyCharacter* ControlledCharacter = Cast<AEnemyCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}


