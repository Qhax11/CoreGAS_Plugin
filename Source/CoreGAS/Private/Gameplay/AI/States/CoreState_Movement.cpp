// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Movement.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "Abilities/GameplayAbilityTypes.h"

UCoreState_Movement::UCoreState_Movement()
{
	StateTag = CoreGAS::AI::TAG_State_Combat_Movement;
}

void UCoreState_Movement::OnEnter(UCoreStateManager* StateManager)
{
	Super::OnEnter(StateManager);

	if (!MovementAbilityClass || !Context.TargetActor || !Context.BehaviorDecision)
	{
		return;
	}

	UCoreASCBase* ASC = Context.OwnerASC;
	if (!ASC)
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.Target = Context.TargetActor;

	const FCoreAttackDataBase* BestAttack = Context.BehaviorDecision->GetBestAttack();
	const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(BestAttack);
	if (SimpleAttack && SimpleAttack->AbilityClass)
	{
		UCoreGameplayAbility_AttackBase* CDO = SimpleAttack->AbilityClass->GetDefaultObject<UCoreGameplayAbility_AttackBase>();
		EventData.EventMagnitude = CDO ? CDO->MaxRange : 150.f;
	}

	if (!ASC->ActivateAbilityByClassAndReturnHandle(MovementAbilityClass, ActiveAbilityHandle, EventData))
	{
		return;
	}

	EndListenerHandle = ASC->ListenForAbilityEndedByHandle(ActiveAbilityHandle, [this](const FAbilityEndedData&)
	{
		if (TransitionTag.IsValid())
		{
			RequestTransition(TransitionTag);
		}
	});
}

void UCoreState_Movement::OnExit(UCoreStateManager* StateManager)
{
	UCoreASCBase* ASC = Context.OwnerASC;
	if (ASC && EndListenerHandle.IsValid())
	{
		ASC->StopListeningForAbilityEnded(EndListenerHandle);
	}

	EndListenerHandle   = FDelegateHandle();
	ActiveAbilityHandle = FGameplayAbilitySpecHandle();

	Super::OnExit(StateManager);
}
