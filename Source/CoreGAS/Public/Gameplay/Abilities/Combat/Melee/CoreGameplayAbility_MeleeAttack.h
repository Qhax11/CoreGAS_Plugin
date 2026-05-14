// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "CoreGameplayAbility_MeleeAttack.generated.h"

class UCoreAbilityTask_PerformTrace;
class UCoreEquipmentComponent;
class ACoreWeaponBase;
class UGameplayEffect;

UCLASS()
class COREGAS_API UCoreGameplayAbility_MeleeAttack : public UCoreGameplayAbility_AttackBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_MeleeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Melee")
	TArray<TSubclassOf<UGameplayEffect>> HitEffects;

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UCoreAbilityTask_PerformTrace>> TraceTasks;

	TSet<TWeakObjectPtr<AActor>> HitActors;

	UFUNCTION()
	void OnTraceHit(const TArray<FHitResult>& HitResults);
};
