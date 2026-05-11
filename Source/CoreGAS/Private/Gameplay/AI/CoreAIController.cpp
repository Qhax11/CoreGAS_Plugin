// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/CoreAIEventHandler.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Tags/CoreAITags.h"

ACoreAIController::ACoreAIController()
{
	StateManager = CreateDefaultSubobject<UCoreStateManager>(TEXT("StateManager"));
	EventHandler = CreateDefaultSubobject<UCoreAIEventHandler>(TEXT("EventHandler"));
}

void ACoreAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UCoreSpawnSubsystem* SpawnSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>())
	{
		SpawnSubsystem->OnHeroSpawn.AddDynamic(this, &ACoreAIController::OnHeroSpawned);
	}
}

void ACoreAIController::OnHeroSpawned(const FHeroSpawnData& HeroSpawnData)
{
	APawn* CachedPawn = GetPawn();
	UCoreASCBase* EnemyASC = CachedPawn ? CachedPawn->FindComponentByClass<UCoreASCBase>() : nullptr;

	EventHandler->Initialize(StateManager, EnemyASC);

	StateManager->Initialize(CachedPawn, EnemyASC, HeroSpawnData.HeroActor);
	StateManager->RequestStateEnter(CoreGAS::AI::TAG_State_Combat_Movement);
}

void ACoreAIController::OnUnPossess()
{
	if (UCoreSpawnSubsystem* SpawnSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>())
	{
		SpawnSubsystem->OnHeroSpawn.RemoveDynamic(this, &ACoreAIController::OnHeroSpawned);
	}

	Super::OnUnPossess();
}