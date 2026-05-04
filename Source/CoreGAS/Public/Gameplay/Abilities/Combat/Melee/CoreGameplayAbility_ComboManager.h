// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "Gameplay/Abilities/Combat/Melee/CoreGameplayAbility_ComboMeleeAttack.h"
#include "CoreGameplayAbility_ComboManager.generated.h"

UCLASS()
class COREGAS_API UCoreGameplayAbility_ComboManager : public UCoreGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_ComboManager();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Combo")
	TArray<TSubclassOf<UCoreGameplayAbility_ComboMeleeAttack>> ComboAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Combo")
	float ComboResetTime = 1.5f;

private:
	int32 CurrentComboIndex = 0;
	FTimerHandle ComboResetTimerHandle;
};
