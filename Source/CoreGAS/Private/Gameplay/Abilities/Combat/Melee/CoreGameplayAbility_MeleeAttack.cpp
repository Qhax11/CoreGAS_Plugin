// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Abilities/Combat/Melee/CoreGameplayAbility_MeleeAttack.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_PerformTrace.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreEquipmentTags.h"
#include "Gameplay/Components/CoreEquipmentComponent.h"
#include "Gameplay/Actors/Weapon/CoreWeaponBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UCoreGameplayAbility_MeleeAttack::UCoreGameplayAbility_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UCoreGameplayAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Melee_TraceBegin_RightHand);
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Melee_TraceBegin_LeftHand);
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Melee_TraceEnd_RightHand);
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Melee_TraceEnd_LeftHand);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCoreGameplayAbility_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	for (auto& [SlotTag, Task] : TraceTasks)
	{
		if (Task)
		{
			Task->EndTask();
		}
	}
	TraceTasks.Empty();
	HitActors.Empty();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCoreGameplayAbility_MeleeAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const bool bIsTraceBegin = EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceBegin_RightHand
		|| EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceBegin_LeftHand;
	const bool bIsTraceEnd = EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceEnd_RightHand
		|| EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceEnd_LeftHand;

	const FGameplayTag SlotTag = (EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceBegin_RightHand
		|| EventTag == CoreGAS::Combat::TAG_Event_Melee_TraceEnd_RightHand)
		? CoreGAS::Equipment::TAG_Equipment_Slot_RightHand
		: CoreGAS::Equipment::TAG_Equipment_Slot_LeftHand;

	if (bIsTraceBegin)
	{
		AActor* AvatarActor = GetAvatarActorFromActorInfo();

		UCoreEquipmentComponent* EquipmentComponent = AvatarActor ? AvatarActor->FindComponentByClass<UCoreEquipmentComponent>() : nullptr;
		if (!EquipmentComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCoreGameplayAbility_MeleeAttack::OnEventReceived - No UCoreEquipmentComponent on avatar actor"));
			return;
		}

		ACoreWeaponBase* Weapon = EquipmentComponent->GetWeaponBySlotTag(SlotTag);
		if (!Weapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCoreGameplayAbility_MeleeAttack::OnEventReceived - No weapon in slot '%s'"), *SlotTag.ToString());
			return;
		}

		if (!Weapon->GetTraceConfig())
		{
			UE_LOG(LogTemp, Warning, TEXT("UCoreGameplayAbility_MeleeAttack::OnEventReceived - Weapon in slot '%s' has no TraceConfig"), *SlotTag.ToString());
			return;
		}

		UCoreAbilityTask_PerformTrace* NewTask = UCoreAbilityTask_PerformTrace::PerformTraceTick(this, Weapon->GetTraceConfig(), MAX_FLT, Weapon);
		TraceTasks.Add(SlotTag, NewTask);
		NewTask->OnHit.AddDynamic(this, &UCoreGameplayAbility_MeleeAttack::OnTraceHit);
		NewTask->ReadyForActivation();
	}
	else if (bIsTraceEnd)
	{
		if (TObjectPtr<UCoreAbilityTask_PerformTrace>* Found = TraceTasks.Find(SlotTag))
		{
			if (*Found)
			{
				(*Found)->EndTask();
			}
			TraceTasks.Remove(SlotTag);
		}
	}
}

void UCoreGameplayAbility_MeleeAttack::OnTraceHit(const TArray<FHitResult>& HitResults)
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
			FGameplayEffectContextHandle Context = SpecHandle.Data->GetContext();
			Context.AddHitResult(Hit, true);  // true = override existing

			if (SpecHandle.IsValid())
			{
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}
		}

		HitActors.Add(HitActor);
	}
}
