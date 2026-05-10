// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_AbilityActivation.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

void UCoreState_AbilityActivation::OnEnter(UCoreStateManager* StateManager)
{
	Super::OnEnter(StateManager);

	if (!AbilityClass)
	{
		return;
	}

	AController* Controller = Cast<AController>(StateManager->GetOwner());
	APawn* Pawn             = Controller ? Controller->GetPawn() : nullptr;
	UCoreASCBase* ASC       = Pawn ? Pawn->FindComponentByClass<UCoreASCBase>() : nullptr;
	if (!ASC)
	{
		return;
	}

	if (!ASC->ActivateAbilityByClassAndReturnHandle(AbilityClass, ActiveAbilityHandle))
	{
		return;
	}

	EndListenerHandle = ASC->ListenForAbilityEndedByHandle(ActiveAbilityHandle, [this](const FAbilityEndedData&)
	{
		OnAbilityEnded();
	});
}

void UCoreState_AbilityActivation::OnExit(UCoreStateManager* StateManager)
{
	AController* Controller = Cast<AController>(StateManager->GetOwner());
	APawn* Pawn             = Controller ? Controller->GetPawn() : nullptr;
	UCoreASCBase* ASC       = Pawn ? Pawn->FindComponentByClass<UCoreASCBase>() : nullptr;
	if (ASC && EndListenerHandle.IsValid())
	{
		ASC->StopListeningForAbilityEnded(EndListenerHandle);
	}

	EndListenerHandle     = FDelegateHandle();
	ActiveAbilityHandle   = FGameplayAbilitySpecHandle();

	Super::OnExit(StateManager);
}

void UCoreState_AbilityActivation::OnAbilityEnded()
{
	if (TransitionTag.IsValid())
	{
		BroadcastTransition(TransitionTag);
	}
}
