// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "CoreGameplayAbility_MeleeAoE.generated.h"

class UCoreTraceConfig;
class UGameplayEffect;
class UDecalComponent;

UCLASS()
class COREGAS_API UCoreGameplayAbility_MeleeAoE : public UCoreGameplayAbility_AttackBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_MeleeAoE();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Melee|AoE")
	TObjectPtr<UCoreTraceConfig> AoETraceConfig;

	UPROPERTY(EditAnywhere, Category = "CoreGAS|Melee|AoE")
	TObjectPtr<UMaterialInterface> AoEDecalMaterial;

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

private:
	TObjectPtr<UDecalComponent> ActiveDecal;
};
