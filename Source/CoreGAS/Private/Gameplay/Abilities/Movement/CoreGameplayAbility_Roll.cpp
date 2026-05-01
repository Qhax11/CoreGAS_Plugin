// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Movement/CoreGameplayAbility_Roll.h"
#include "Gameplay/Tags/CoreAbilityTags.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"

UAnimMontage* UCoreRollDataAsset::FindMontage(FGameplayTag DirectionTag) const
{
	for (const FCoreDirectionRollPair& Pair : DirectionRollPairs)
	{
		if (Pair.DirectionTag == DirectionTag)
		{
			return Pair.MontageData.Montage.LoadSynchronous();
		}
	}
	return nullptr;
}

float UCoreRollDataAsset::FindWarpDistance(FGameplayTag DirectionTag) const
{
	for (const FCoreDirectionRollPair& Pair : DirectionRollPairs)
	{
		if (Pair.DirectionTag == DirectionTag)
		{
			return Pair.MontageData.MotionWarpingDistance;
		}
	}
	return 300.f;
}

UCoreGameplayAbility_Roll::UCoreGameplayAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	ActivationBlockedTags.AddTag(CoreGAS::Ability::TAG_Ability_Phase_Active);
	bCommitCooldownOnActivate = false;
	bCommitCostOnActivate = false;
}

void UCoreGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!RollDataAsset)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreGameplayAbility_Roll::OnAfterFrame - No avatar actor found."));
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), false, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(AvatarActor);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreGameplayAbility_Roll::OnAfterFrame - Avatar is not a Character on %s."), *AvatarActor->GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), false, true);
		return;
	}

	if (Character && Character->GetCharacterMovement())
	{
		const FVector WorldInput = Character->GetLastMovementInputVector();
		const FVector LocalInput = Character->GetActorRotation().UnrotateVector(WorldInput);
		CachedInputDirection = FVector2D(LocalInput.X, LocalInput.Y);
	}

	const FGameplayTag DirectionTag = GetDirectionTagFromInput(CachedInputDirection);
	UAnimMontage* Montage = RollDataAsset->FindMontage(DirectionTag);
	if (!Montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreGameplayAbility_Roll::OnAfterFrame - No montage found for direction '%s' on %s."),
			*DirectionTag.ToString(), *AvatarActor->GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	const float WarpDistance = RollDataAsset->FindWarpDistance(DirectionTag);

	UMotionWarpingComponent* MotionWarpingComp = AvatarActor->FindComponentByClass<UMotionWarpingComponent>();
	if (MotionWarpingComp)
	{
		FVector InputDir = FVector(CachedInputDirection.X, CachedInputDirection.Y, 0.f).GetSafeNormal();
		if (InputDir.IsNearlyZero())
		{
			InputDir = AvatarActor->GetActorForwardVector();
		}

		FMotionWarpingTarget WarpTarget;
		WarpTarget.Name = WarpTargetName;
		WarpTarget.Location = AvatarActor->GetActorLocation() + InputDir * WarpDistance;
		MotionWarpingComp->AddOrUpdateWarpTarget(WarpTarget);
	}

	AbilityMontage = Montage;
	Super::ActivateAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), nullptr);

	WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DamageImmunityDuration);
	WaitDelayTask->OnFinish.AddDynamic(this, &UCoreGameplayAbility_Roll::RemoveDamageImmunity);
	WaitDelayTask->ReadyForActivation();
}

FGameplayTag UCoreGameplayAbility_Roll::GetDirectionTagFromInput(const FVector2D& Input) const
{
	if (Input.IsNearlyZero())
	{
		return CoreGAS::Ability::TAG_Ability_Direction_Forward;
	}

	if (FMath::Abs(Input.X) >= FMath::Abs(Input.Y))
	{
		return Input.X >= 0.f ? CoreGAS::Ability::TAG_Ability_Direction_Forward : CoreGAS::Ability::TAG_Ability_Direction_Backward;
	}
	else
	{
		return Input.Y >= 0.f ? CoreGAS::Ability::TAG_Ability_Direction_Right : CoreGAS::Ability::TAG_Ability_Direction_Left;
	}
}

void UCoreGameplayAbility_Roll::RemoveDamageImmunity()
{
	WaitDelayTask = nullptr;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(CoreGAS::Character::TAG_Status_Invincible);
	}
}

void UCoreGameplayAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitDelayTask)
	{
		WaitDelayTask->EndTask();
		WaitDelayTask = nullptr;
	}

	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(CoreGAS::Ability::TAG_Ability_Phase_Active);
		ASC->RemoveLooseGameplayTag(CoreGAS::Character::TAG_Status_Invincible);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
