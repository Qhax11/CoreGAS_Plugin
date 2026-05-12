// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Dead.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "Gameplay/Tags/CoreCombatTags.h"

UCoreState_Dead::UCoreState_Dead()
{
	StateTag = CoreGAS::AI::TAG_State_Dead;
}

void UCoreState_Dead::OnEnter(UCoreStateManager* StateManager)
{
	Super::OnEnter(StateManager);

	if (!Context.OwnerActor || !Context.OwnerASC)
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Context.OwnerActor,
		CoreGAS::Combat::TAG_Event_Death,
		FGameplayEventData()
	);
}
