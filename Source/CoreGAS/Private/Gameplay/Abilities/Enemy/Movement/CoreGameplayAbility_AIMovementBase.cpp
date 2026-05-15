// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"
#include "Gameplay/Tags/CoreAITags.h"

UCoreGameplayAbility_AIMovementBase::UCoreGameplayAbility_AIMovementBase()
{
	AbilityTags.AddTag(CoreGAS::AI::TAG_AI_Ability_Movement);
	bCommitCooldownOnActivate = false;
	bCommitCostOnActivate = false;
}

void UCoreGameplayAbility_AIMovementBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MoveTask)
	{
		MoveTask->EndTask();
		MoveTask = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCoreGameplayAbility_AIMovementBase::OnMoveCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UCoreGameplayAbility_AIMovementBase::OnMoveAborted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UCoreGameplayAbility_AIMovementBase::OnMoveFailed()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}
