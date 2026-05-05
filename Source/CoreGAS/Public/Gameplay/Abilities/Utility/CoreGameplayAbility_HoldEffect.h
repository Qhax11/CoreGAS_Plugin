// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "CoreGameplayAbility_HoldEffect.generated.h"

class UGameplayEffect;

UCLASS()
class COREGAS_API UCoreGameplayAbility_HoldEffect : public UCoreGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_HoldEffect();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|HoldEffect")
	TSubclassOf<UGameplayEffect> EffectToApply;

private:
	FActiveGameplayEffectHandle ActiveEffectHandle;
};
