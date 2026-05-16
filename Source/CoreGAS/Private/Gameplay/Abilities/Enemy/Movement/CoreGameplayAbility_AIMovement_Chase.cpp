// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_Chase.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "AIController.h"

void UCoreGameplayAbility_AIMovement_Chase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AAIController* AIController = Avatar ? Cast<AAIController>(Avatar->GetInstigatorController()) : nullptr;
	if (!AIController)
	{
		EndReason = ECoreMovementEndReason::SetupFailed;
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AActor* Target = TriggerEventData ? const_cast<AActor*>(TriggerEventData->Target.Get()) : nullptr;
	if (!Target)
	{
		EndReason = ECoreMovementEndReason::SetupFailed;
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	const float Radius = (TriggerEventData && TriggerEventData->EventMagnitude > 0.f)
		? TriggerEventData->EventMagnitude
		: AcceptanceRadius;

	const FCoreMovementConfig_Chase* ChaseConfig = (TriggerEventData && TriggerEventData->TargetData.IsValid(0))
		? static_cast<const FCoreMovementConfig_Chase*>(TriggerEventData->TargetData.Get(0))
		: nullptr;
	float ChaseTime     = ChaseConfig ? ChaseConfig->MaxChaseTime     : 0.f;
	float ChaseDistance = ChaseConfig ? ChaseConfig->MaxChaseDistance : 0.f;

	MoveTask = UCoreAbilityTask_AIMoveTo::CreateAIMoveToActor(this, AIController, Target, Radius, ChaseTime, ChaseDistance);
	MoveTask->OnCompleted.AddDynamic(this,  &UCoreGameplayAbility_AIMovement_Chase::OnMoveCompleted);
	MoveTask->OnAborted.AddDynamic(this,    &UCoreGameplayAbility_AIMovement_Chase::OnMoveAborted);
	MoveTask->OnFailed.AddDynamic(this,     &UCoreGameplayAbility_AIMovement_Chase::OnMoveFailed);
	MoveTask->OnCancelled.AddDynamic(this,  &UCoreGameplayAbility_AIMovement_Chase::OnMoveCancelled);
	MoveTask->ReadyForActivation();
}

void UCoreGameplayAbility_AIMovement_Chase::OnMoveCancelled()
{
	EndReason = ECoreMovementEndReason::Cancelled;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}
