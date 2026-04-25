// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Abilities/Combat/CoreAbility_Melee.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_PerformTrace.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCoreAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Melee_TraceBegin);
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Melee_TraceEnd);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCoreAbility_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (TraceTask)
	{
		TraceTask->EndTask();
		TraceTask = nullptr;
	}
	HitActors.Empty();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCoreAbility_Melee::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceBegin)
	{
		TraceTask = UCoreAbilityTask_PerformTrace::PerformTraceTick(this, TraceConfig, MAX_FLT);
		TraceTask->OnHit.AddDynamic(this, &UCoreAbility_Melee::OnTraceHit);
		TraceTask->ReadyForActivation();
	}
	else if (EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceEnd)
	{
		if (TraceTask)
		{
			TraceTask->EndTask();
			TraceTask = nullptr;
		}
	}
}

void UCoreAbility_Melee::OnTraceHit(const TArray<FHitResult>& HitResults)
{
	if (HitEffects.IsEmpty())
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		return;
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActors.Contains(HitActor))
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)
		{
			continue;
		}

		for (const TSubclassOf<UGameplayEffect>& EffectClass : HitEffects)
		{
			if (!EffectClass)
			{
				continue;
			}

			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
			if (SpecHandle.IsValid())
			{
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}
		}

		HitActors.Add(HitActor);
	}
}
