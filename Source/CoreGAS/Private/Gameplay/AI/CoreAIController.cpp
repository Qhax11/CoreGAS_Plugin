// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/CoreAIEventHandler.h"
#include "AbilitySystemComponent.h"

ACoreAIController::ACoreAIController()
{
	StateManager  = CreateDefaultSubobject<UCoreStateManager>(TEXT("StateManager"));
	EventHandler  = CreateDefaultSubobject<UCoreAIEventHandler>(TEXT("EventHandler"));
}

void ACoreAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StateManager->CreateStates();

	UAbilitySystemComponent* EnemyASC = InPawn->FindComponentByClass<UAbilitySystemComponent>();
	EventHandler->Initialize(StateManager, EnemyASC);
}
