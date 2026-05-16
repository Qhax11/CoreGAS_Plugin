// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_Chase.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"
#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
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

	float ChaseTime = 0.f;
	float ChaseDistance = 0.f;
	if (ACoreAIController* CoreAIController = Cast<ACoreAIController>(AIController))
	{
		if (UCoreAIBehaviorDecision* BD = CoreAIController->GetBehaviorDecision())
		{
			if (const FCoreAttackDataBase* SelectedAttack = BD->GetSelectedAttack())
			{
				if (const FCoreMovementConfig_Chase* ChaseConfig = SelectedAttack->MovementConfig.GetPtr<FCoreMovementConfig_Chase>())
				{
					ChaseTime = ChaseConfig->MaxChaseTime;
					ChaseDistance = ChaseConfig->MaxChaseDistance;
				}
			}
		}
	}

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
