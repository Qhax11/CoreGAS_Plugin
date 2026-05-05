// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Abilities/Utility/CoreGameplayAbility_HoldEffect.h"

UCoreGameplayAbility_HoldEffect::UCoreGameplayAbility_HoldEffect()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UCoreGameplayAbility_HoldEffect::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (EffectToApply)
	{
		ActiveEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo,
			EffectToApply->GetDefaultObject<UGameplayEffect>(), GetAbilityLevel());
	}
}

void UCoreGameplayAbility_HoldEffect::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UCoreGameplayAbility_HoldEffect::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActiveEffectHandle.IsValid())
	{
		BP_RemoveGameplayEffectFromOwnerWithHandle(ActiveEffectHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
