// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Enemy/CoreGameplayAbility_AIMovement.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "AIController.h"

UCoreGameplayAbility_AIMovement::UCoreGameplayAbility_AIMovement()
{
	AbilityTags.AddTag(CoreGAS::AI::TAG_AI_Ability_Movement);
	bCommitCooldownOnActivate = false;
	bCommitCostOnActivate = false;
}

void UCoreGameplayAbility_AIMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AAIController* AIController = Avatar ? Cast<AAIController>(Avatar->GetInstigatorController()) : nullptr;
	if (!AIController)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AActor* Target = TriggerEventData ? const_cast<AActor*>(TriggerEventData->Target.Get()) : nullptr;
	if (!Target)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	MoveTask = UCoreAbilityTask_AIMoveTo::CreateAIMoveToActor(this, AIController, Target, AcceptanceRadius);
	MoveTask->OnCompleted.AddDynamic(this, &UCoreGameplayAbility_AIMovement::OnMoveCompleted);
	MoveTask->OnAborted.AddDynamic(this,   &UCoreGameplayAbility_AIMovement::OnMoveAborted);
	MoveTask->OnFailed.AddDynamic(this,    &UCoreGameplayAbility_AIMovement::OnMoveFailed);
	MoveTask->ReadyForActivation();
}

void UCoreGameplayAbility_AIMovement::EndAbility(const FGameplayAbilitySpecHandle Handle,
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

void UCoreGameplayAbility_AIMovement::OnMoveCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UCoreGameplayAbility_AIMovement::OnMoveAborted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UCoreGameplayAbility_AIMovement::OnMoveFailed()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}
