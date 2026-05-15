// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"
#include "Gameplay/Debug/CoreGameplayLog.h"

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

UCoreAbilityTask_AIMoveTo* UCoreAbilityTask_AIMoveTo::CreateAIMoveToLocation(
	UGameplayAbility* OwningAbility,
	AAIController* Controller,
	FVector Location,
	float AcceptanceRadius)
{
	UCoreAbilityTask_AIMoveTo* Task = NewAbilityTask<UCoreAbilityTask_AIMoveTo>(OwningAbility);
	Task->CachedController       = Controller;
	Task->CachedLocation         = Location;
	Task->CachedAcceptanceRadius = AcceptanceRadius;
	Task->bMoveToLocation        = true;
	return Task;
}

void UCoreAbilityTask_AIMoveTo::Activate()
{
	if (!CachedController || (!bMoveToLocation && !CachedTarget))
	{
		UE_LOG(LogCoreAIMovement, Warning, TEXT("[AIMoveTo] Activate FAILED — Controller:%s Target:%s"),
			CachedController ? TEXT("OK") : TEXT("NULL"),
			CachedTarget ? TEXT("OK") : TEXT("NULL"));
		if (ShouldBroadcastAbilityTaskDelegates()) OnFailed.Broadcast();
		EndTask();
		return;
	}

	CachedController->ReceiveMoveCompleted.AddDynamic(this, &UCoreAbilityTask_AIMoveTo::OnMoveCompleted);

	EPathFollowingRequestResult::Type Result;
	if (bMoveToLocation)
	{
		Result = CachedController->MoveToLocation(CachedLocation, CachedAcceptanceRadius);
		UE_LOG(LogCoreAIMovement, Log, TEXT("[AIMoveTo] MoveToLocation started — Location:%s Radius:%.1f"),
			*CachedLocation.ToString(), CachedAcceptanceRadius);
	}
	else
	{
		Result = CachedController->MoveToActor(CachedTarget, CachedAcceptanceRadius);
		UE_LOG(LogCoreAIMovement, Log, TEXT("[AIMoveTo] MoveToActor started — Target:%s Radius:%.1f"),
			*CachedTarget->GetName(), CachedAcceptanceRadius);
	}

	if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		UE_LOG(LogCoreAIMovement, Log, TEXT("[AIMoveTo] AlreadyAtGoal — broadcasting Completed"));
		CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UCoreAbilityTask_AIMoveTo::OnMoveCompleted);
		if (ShouldBroadcastAbilityTaskDelegates()) OnCompleted.Broadcast();
		EndTask();
		return;
	}

	if (Result == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogCoreAIMovement, Warning, TEXT("[AIMoveTo] Move FAILED"));
		CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UCoreAbilityTask_AIMoveTo::OnMoveCompleted);
		if (ShouldBroadcastAbilityTaskDelegates()) OnFailed.Broadcast();
		EndTask();
		return;
	}
}

void UCoreAbilityTask_AIMoveTo::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	UE_LOG(LogCoreAIMovement, Log, TEXT("[AIMoveTo] OnMoveCompleted � Result:%d"), static_cast<int32>(Result));

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
