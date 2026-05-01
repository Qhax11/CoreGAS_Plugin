// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Combat/CoreAbility_ComboManager.h"
#include "Gameplay/Tags/CoreAbilityTags.h"
#include "AbilitySystemComponent.h"

UCoreAbility_ComboManager::UCoreAbility_ComboManager()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(CoreGAS::Ability::TAG_Ability_Phase_Active);
}

void UCoreAbility_ComboManager::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ComboAbilities.IsEmpty() || !ComboAbilities.IsValidIndex(CurrentComboIndex))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Index: %d"), CurrentComboIndex);
	ASC->TryActivateAbilityByClass(ComboAbilities[CurrentComboIndex]);

	CurrentComboIndex++;
	if (CurrentComboIndex >= ComboAbilities.Num())
	{
		CurrentComboIndex = 0;
	}

	UWorld* World = ActorInfo->OwnerActor.IsValid() ? ActorInfo->OwnerActor->GetWorld() : nullptr;
	if (World)
	{
		World->GetTimerManager().ClearTimer(ComboResetTimerHandle);
		World->GetTimerManager().SetTimer(ComboResetTimerHandle, [this]()
		{
			CurrentComboIndex = 0;
		}, ComboResetTime, false);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UCoreAbility_ComboManager::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
