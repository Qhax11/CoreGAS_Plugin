// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreAIEventHandler.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/Tags/CoreAttributeTags.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "AbilitySystemComponent.h"

void UCoreAIEventHandler::Initialize(UCoreStateManager* InStateManager, UAbilitySystemComponent* InASC)
{
	StateManager = InStateManager;
	CachedASC    = InASC;

	if (InASC)
	{
		InASC->RegisterGameplayTagEvent(
			CoreGAS::Attribute::TAG_Attribute_Health_Empty,
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &UCoreAIEventHandler::OnHealthEmptyTagChanged);
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

	Super::EndPlay(EndPlayReason);
}

void UCoreAIEventHandler::OnHealthEmptyTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0 && StateManager)
	{
		StateManager->RequestStateEnter(CoreGAS::AI::TAG_State_Death);
	}
}
