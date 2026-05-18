// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Abilities/Combat/Melee/CoreGameplayAbility_MeleeAoE.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "Gameplay/Tracing/CoreTraceConfig.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UCoreGameplayAbility_MeleeAoE::UCoreGameplayAbility_MeleeAoE()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_AoE_Impact);
}

void UCoreGameplayAbility_MeleeAoE::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!AoETraceConfig)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
}

void UCoreGameplayAbility_MeleeAoE::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag != CoreGAS::Combat::TAG_Event_AoE_Impact)
	{
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	TArray<FHitResult> HitResults = AoETraceConfig->Execute(AvatarActor, FVector::ZeroVector, FVector::ZeroVector, AvatarActor);
	if (HitEffects.IsEmpty())
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		return;
	}
	const bool bSourceIsEnemy = SourceASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Entity_Enemy);

	TSet<AActor*> HitActors;
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

		if (bSourceIsEnemy && TargetASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Entity_Enemy))
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
			if (!SpecHandle.IsValid())
			{
				continue;
			}

			FGameplayEffectContextHandle Context = SpecHandle.Data->GetContext();
			Context.AddHitResult(Hit, true);
			SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}

		HitActors.Add(HitActor);
	}
}