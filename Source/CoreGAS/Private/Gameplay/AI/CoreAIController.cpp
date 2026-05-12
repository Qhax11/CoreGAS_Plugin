// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/CoreAIEventHandler.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreEnemyArchetypeData.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "AbilitySystemComponent.h"

ACoreAIController::ACoreAIController()
{
	StateManager      = CreateDefaultSubobject<UCoreStateManager>(TEXT("StateManager"));
	EventHandler      = CreateDefaultSubobject<UCoreAIEventHandler>(TEXT("EventHandler"));
	BehaviorDecision  = CreateDefaultSubobject<UCoreAIBehaviorDecision>(TEXT("BehaviorDecision"));
}

void ACoreAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("ACoreAIController OnPossess:"));

	if (UCoreSpawnSubsystem* SpawnSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>())
	{
		if (SpawnSubsystem->IsHeroSpawned())
		{
			OnHeroSpawned(SpawnSubsystem->GetLastHeroSpawnData());
		}
		else
		{
			SpawnSubsystem->OnHeroSpawn.AddDynamic(this, &ACoreAIController::OnHeroSpawned);
		}
	}
}

void ACoreAIController::OnHeroSpawned(const FHeroSpawnData& HeroSpawnData)
{
	APawn* CachedPawn = GetPawn();
	UCoreASCBase* EnemyASC = CachedPawn ? CachedPawn->FindComponentByClass<UCoreASCBase>() : nullptr;

	EventHandler->Initialize(StateManager, EnemyASC);

	StateManager->Initialize(CachedPawn, EnemyASC, HeroSpawnData.HeroActor);
	StateManager->RequestStateEnter(CoreGAS::AI::TAG_State_Combat_Movement);

	UAbilitySystemComponent* TargetASC = Cast<UAbilitySystemComponent>(HeroSpawnData.HeroActor->FindComponentByClass<UAbilitySystemComponent>());
	BehaviorDecision->Initialize(CachedPawn, EnemyASC, HeroSpawnData.HeroActor, TargetASC);
}

void ACoreAIController::InitializeBehavior(UCoreEnemyArchetypeData* ArchetypeData)
{
	if (!ArchetypeData)
	{
		return;
	}

	BehaviorDecision->SetArchetypeData(ArchetypeData);
	StateManager->StartLogic();
}

void ACoreAIController::OnUnPossess()
{
	if (UCoreSpawnSubsystem* SpawnSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>())
	{
		SpawnSubsystem->OnHeroSpawn.RemoveDynamic(this, &ACoreAIController::OnHeroSpawned);
	}

	Super::OnUnPossess();
}