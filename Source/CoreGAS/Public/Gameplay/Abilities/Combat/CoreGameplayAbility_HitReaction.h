// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "CoreGameplayAbility_HitReaction.generated.h"

UCLASS()
class COREGAS_API UCoreGameplayAbility_HitReaction : public UCoreGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_HitReaction();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|HitReaction")
	float KnockbackForce = 500.f;
};
