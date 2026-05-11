// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIEventHandler.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/Tags/CoreAttributeTags.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "AbilitySystemComponent.h"
#include "Engine/GameInstance.h"

void UCoreAIEventHandler::Initialize(UCoreStateManager* InStateManager, UAbilitySystemComponent* InASC, UGameInstance* InGameInstance)
{
	CachedStateManager = InStateManager;
	CachedASC          = InASC;

	if (InASC)
	{
		InASC->RegisterGameplayTagEvent(
			CoreGAS::Attribute::TAG_Attribute_Health_Empty,
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &UCoreAIEventHandler::OnHealthEmptyTagChanged);
	}

	if (UCoreSpawnSubsystem* SpawnSubsystem = InGameInstance ? InGameInstance->GetSubsystem<UCoreSpawnSubsystem>() : nullptr)
	{
		SpawnSubsystem->OnHeroSpawn.AddDynamic(this, &UCoreAIEventHandler::OnHeroSpawned);
	}
}

void UCoreAIEventHandler::OnHeroSpawned(const FHeroSpawnData& HeroSpawnData)
{
	CachedHeroActor = HeroSpawnData.HeroActor;

	if (CachedStateManager)
	{
		CachedStateManager->StartLogic();
	}
}

void UCoreAIEventHandler::OnHealthEmptyTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0 && CachedStateManager)
	{
		CachedStateManager->RequestStateEnter(CoreGAS::AI::TAG_State_Death);
	}
}

void UCoreAIEventHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CachedASC.IsValid())
	{
		CachedASC->RegisterGameplayTagEvent(
			CoreGAS::Attribute::TAG_Attribute_Health_Empty,
			EGameplayTagEventType::NewOrRemoved
		).RemoveAll(this);
	}

	if (UWorld* World = GetWorld())
	{
		if (UCoreSpawnSubsystem* SpawnSubsystem = World->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>())
		{
			SpawnSubsystem->OnHeroSpawn.RemoveDynamic(this, &UCoreAIEventHandler::OnHeroSpawned);
		}
	}

	Super::EndPlay(EndPlayReason);
}
