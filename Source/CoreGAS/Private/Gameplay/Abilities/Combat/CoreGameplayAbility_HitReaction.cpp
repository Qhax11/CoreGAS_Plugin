// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Combat/CoreGameplayAbility_HitReaction.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "GameFramework/Character.h"

UCoreGameplayAbility_HitReaction::UCoreGameplayAbility_HitReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = CoreGAS::Combat::TAG_Event_HitReaction;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UCoreGameplayAbility_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
	{
		FVector KnockbackDirection = FVector::ZeroVector;
		if (TriggerEventData && TriggerEventData->Instigator)
		{
			KnockbackDirection = (OwnerActor->GetActorLocation() - TriggerEventData->Instigator->GetActorLocation()).GetSafeNormal();
			KnockbackDirection.Z = 0.f;
			KnockbackDirection = KnockbackDirection.GetSafeNormal();
		}
		OwnerCharacter->LaunchCharacter(KnockbackDirection * KnockbackForce, true, false);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
