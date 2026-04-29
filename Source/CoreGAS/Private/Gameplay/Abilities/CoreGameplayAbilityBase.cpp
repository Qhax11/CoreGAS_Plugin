d// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"

UCoreGameplayAbilityBase::UCoreGameplayAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UCoreGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bCommitCooldownOnActivate)
	{
		CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true);
	}

	if (bCommitCostOnActivate)
	{
		CommitAbilityCost(Handle, ActorInfo, ActivationInfo);
	}
}

