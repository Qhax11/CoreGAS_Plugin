// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"

UCoreAbilityTask_AIMoveTo* UCoreAbilityTask_AIMoveTo::CreateAIMoveToActor(
	UGameplayAbility* OwningAbility,
	AAIController* Controller,
	AActor* TargetActor,
	float AcceptanceRadius)
{
	UCoreAbilityTask_AIMoveTo* Task = NewAbilityTask<UCoreAbilityTask_AIMoveTo>(OwningAbility);
	Task->CachedController       = Controller;
	Task->CachedTarget           = TargetActor;
	Task->CachedAcceptanceRadius = AcceptanceRadius;
	return Task;
}

void UCoreAbilityTask_AIMoveTo::Activate()
{
	if (!CachedController || !CachedTarget)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFailed.Broadcast();
		}
		EndTask();
		return;
	}

	CachedController->ReceiveMoveCompleted.AddDynamic(this, &UCoreAbilityTask_AIMoveTo::OnMoveCompleted);

	const EPathFollowingRequestResult::Type Result =
		CachedController->MoveToActor(CachedTarget, CachedAcceptanceRadius);

	if (Result == EPathFollowingRequestResult::Failed)
	{
		CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UCoreAbilityTask_AIMoveTo::OnMoveCompleted);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFailed.Broadcast();
		}
		EndTask();
	}
}

void UCoreAbilityTask_AIMoveTo::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		EndTask();
		return;
	}

	switch (Result)
	{
	case EPathFollowingResult::Success:
		OnCompleted.Broadcast();
		break;
	case EPathFollowingResult::Aborted:
		OnAborted.Broadcast();
		break;
	default:
		OnFailed.Broadcast();
		break;
	}

	EndTask();
}

void UCoreAbilityTask_AIMoveTo::OnDestroy(bool AbilityEnded)
{
	if (CachedController)
	{
		CachedController->StopMovement();
		CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UCoreAbilityTask_AIMoveTo::OnMoveCompleted);
	}

	Super::OnDestroy(AbilityEnded);
}
