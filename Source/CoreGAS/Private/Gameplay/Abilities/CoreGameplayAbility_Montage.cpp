// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/CoreGameplayAbility_Montage.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UCoreGameplayAbility_Montage::UCoreGameplayAbility_Montage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UCoreGameplayAbility_Montage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AbilityMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageTask = UCoreAbilityTask_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(
		this, NAME_None, AbilityMontage, WaitForEventTags, PlayRate, StartSection);

	MontageTask->OnCompleted.AddDynamic(this, &UCoreGameplayAbility_Montage::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UCoreGameplayAbility_Montage::OnMontageBlendOut);
	MontageTask->OnInterrupted.AddDynamic(this, &UCoreGameplayAbility_Montage::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UCoreGameplayAbility_Montage::OnMontageCancelled);
	MontageTask->EventReceived.AddDynamic(this, &UCoreGameplayAbility_Montage::OnEventReceived);

	MontageTask->ReadyForActivation();
}

void UCoreGameplayAbility_Montage::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MontageTask)
	{
		MontageTask->OnCompleted.RemoveAll(this);
		MontageTask->OnBlendOut.RemoveAll(this);
		MontageTask->OnInterrupted.RemoveAll(this);
		MontageTask->OnCancelled.RemoveAll(this);
		MontageTask->EventReceived.RemoveAll(this);
		MontageTask->EndTask();
		MontageTask = nullptr;
	}

	CancelBlendOutDelay();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCoreGameplayAbility_Montage::OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (PostBlendOutDelay > 0.f && MontageEndPolicy == EMontageEndPolicy::Standard)
	{
		BlendOutDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, PostBlendOutDelay);
		BlendOutDelayTask->OnFinish.AddDynamic(this, &UCoreGameplayAbility_Montage::OnBlendOutDelayFinished);
		BlendOutDelayTask->ReadyForActivation();
	}
	else if (MontageEndPolicy == EMontageEndPolicy::Standard)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UCoreGameplayAbility_Montage::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(false);
}

void UCoreGameplayAbility_Montage::OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(true);
}

void UCoreGameplayAbility_Montage::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	HandleMontageEvent(true);
}

void UCoreGameplayAbility_Montage::OnBlendOutDelayFinished()
{
	BlendOutDelayTask = nullptr;
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UCoreGameplayAbility_Montage::HandleMontageEvent(bool bWasCancelled)
{
	switch (MontageEndPolicy)
	{
	case EMontageEndPolicy::Standard:
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, bWasCancelled);
		break;

	case EMontageEndPolicy::Never:
	default:
		break;
	}
}

void UCoreGameplayAbility_Montage::CancelBlendOutDelay()
{
	if (BlendOutDelayTask)
	{
		BlendOutDelayTask->EndTask();
		BlendOutDelayTask = nullptr;
	}
}
