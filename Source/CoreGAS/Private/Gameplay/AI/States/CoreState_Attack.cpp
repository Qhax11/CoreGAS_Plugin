// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Attack.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Tags/CoreAITags.h"

UCoreState_Attack::UCoreState_Attack()
{
	StateTag = CoreGAS::AI::TAG_State_Combat_Attack;
}

void UCoreState_Attack::OnEnter(UCoreStateManager* StateManager)
{
	UCoreAIBehaviorDecision* BD = Context.BehaviorDecision;

	if (!BD)
	{
		Super::OnEnter(StateManager);
		return;
	}

	const FCoreAttackDataBase* BestAttack = BD->GetSelectedAttack();

	if (!BestAttack)
	{
		Super::OnEnter(StateManager);
		return;
	}

	const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(BestAttack);

	if (!SimpleAttack || !SimpleAttack->AbilityClass)
	{
		Super::OnEnter(StateManager);
		return;
	}

	AbilityClass = SimpleAttack->AbilityClass;
	Super::OnEnter(StateManager);
}

void UCoreState_Attack::OnAbilityEnded()
{
	if (Context.BehaviorDecision)
	{
		RequestTransition(Context.BehaviorDecision->DecideNextState());
	}
}
