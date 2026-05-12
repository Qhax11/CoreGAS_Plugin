// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Movement.h"
#include "Gameplay/AI/CoreStateManager.h"
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

	if (!MovementAbilityClass || !Context.TargetActor)
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
