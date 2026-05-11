// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Movement.h"
#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "AIController.h"

UCoreState_Movement::UCoreState_Movement()
{
	StateTag = CoreGAS::AI::TAG_State_Combat_Movement;
}

void UCoreState_Movement::OnEnter(UCoreStateManager* StateManager)
{
	Super::OnEnter(StateManager);

	if (!MovementAbilityClass)
	{
		return;
	}

	ACoreAIController* AIController = Cast<ACoreAIController>(StateManager->GetOwner());
	if (!AIController)
	{
		return;
	}

	if (!AIController->GetFocusActor())
	{
		return;
	}

	UCoreASCBase* ASC = UCoreGASLibrary::GetCoreASCFromActor(AIController->GetPawn());
	if (!ASC)
	{
		return;
	}

	if (!ASC->ActivateAbilityByClassAndReturnHandle(MovementAbilityClass, ActiveAbilityHandle))
	{
		return;
	}

	EndListenerHandle = ASC->ListenForAbilityEndedByHandle(ActiveAbilityHandle, [this](const FAbilityEndedData&)
	{
		if (TransitionTag.IsValid())
		{
			BroadcastTransition(TransitionTag);
		}
	});
}

void UCoreState_Movement::OnExit(UCoreStateManager* StateManager)
{
	ACoreAIController* AIController = Cast<ACoreAIController>(StateManager->GetOwner());
	UCoreASCBase* ASC = AIController
		? UCoreGASLibrary::GetCoreASCFromActor(AIController->GetPawn())
		: nullptr;

	if (ASC && EndListenerHandle.IsValid())
	{
		ASC->StopListeningForAbilityEnded(EndListenerHandle);
	}

	EndListenerHandle   = FDelegateHandle();
	ActiveAbilityHandle = FGameplayAbilitySpecHandle();

	Super::OnExit(StateManager);
}
