// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/CoreStateManager.h"

ACoreAIController::ACoreAIController()
{
	StateManager = CreateDefaultSubobject<UCoreStateManager>(TEXT("StateManager"));
}

void ACoreAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StateManager->CreateStates();
}
