// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Combat/CoreGameplayAbility_HitReaction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "GameFramework/Character.h"

UCoreGameplayAbility_HitReaction::UCoreGameplayAbility_HitReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = CoreGAS::Combat::TAG_Event_HitReaction;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	bCommitCooldownOnActivate = false;
	bCommitCostOnActivate = false;
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

		FVector CurrentVelocity = OwnerCharacter->GetCharacterMovement()->Velocity;
		CurrentVelocity.X = KnockbackDirection.X * KnockbackForce;
		CurrentVelocity.Y = KnockbackDirection.Y * KnockbackForce;

		OwnerCharacter->GetCharacterMovement()->Velocity = CurrentVelocity;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
