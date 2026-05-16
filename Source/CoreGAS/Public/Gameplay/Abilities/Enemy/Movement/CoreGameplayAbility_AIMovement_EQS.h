// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CoreGameplayAbility_AIMovement_EQS.generated.h"

UCLASS()
class COREGAS_API UCoreGameplayAbility_AIMovement_EQS : public UCoreGameplayAbility_AIMovementBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);
};
