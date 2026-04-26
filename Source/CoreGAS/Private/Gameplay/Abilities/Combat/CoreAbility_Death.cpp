// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Combat/CoreAbility_Death.h"
#include "Gameplay/Abilities/CoreGameplayAbility_Montage.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "AbilitySystemComponent.h"

UCoreAbility_Death::UCoreAbility_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = CoreGAS::Combat::TAG_Event_Death;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UCoreAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(CoreGAS::Character::TAG_Status_Dead);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
