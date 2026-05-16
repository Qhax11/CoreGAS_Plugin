// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/CoreAIEventHandler.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/Debug/CoreAIDebugComponent.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "AbilitySystemComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

ACoreAIController::ACoreAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	StateManager      = CreateDefaultSubobject<UCoreStateManager>(TEXT("StateManager"));
	EventHandler      = CreateDefaultSubobject<UCoreAIEventHandler>(TEXT("EventHandler"));
	BehaviorDecision  = CreateDefaultSubobject<UCoreAIBehaviorDecision>(TEXT("BehaviorDecision"));
	DebugComponent    = CreateDefaultSubobject<UCoreAIDebugComponent>(TEXT("DebugComponent"));
}

void ACoreAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(600);
	}

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
	UCoreASCBase* OwnerASC = CachedPawn ? CachedPawn->FindComponentByClass<UCoreASCBase>() : nullptr;

	CurrentTarget = HeroSpawnData.HeroActor;

	UAbilitySystemComponent* TargetASC = Cast<UAbilitySystemComponent>(HeroSpawnData.HeroActor->FindComponentByClass<UAbilitySystemComponent>());

	BehaviorDecision->Initialize(CachedPawn, OwnerASC, HeroSpawnData.HeroActor, TargetASC);
	EventHandler->Initialize(StateManager, OwnerASC);
	StateManager->Initialize(CachedPawn, OwnerASC, HeroSpawnData.HeroActor, BehaviorDecision);
	DebugComponent->Initialize(CachedPawn, StateManager, BehaviorDecision);
}

void ACoreAIController::OnUnPossess()
{
	CurrentTarget = nullptr;

	if (UCoreSpawnSubsystem* SpawnSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>())
	{
		SpawnSubsystem->OnHeroSpawn.RemoveDynamic(this, &ACoreAIController::OnHeroSpawned);
	}

	Super::OnUnPossess();
}

AActor* ACoreAIController::GetCurrentTarget() const
{
	return CurrentTarget;
}