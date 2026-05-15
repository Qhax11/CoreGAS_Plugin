// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CoreGameplayAbility_AIMovement_EQS.generated.h"

class UEnvQuery;

UCLASS()
class COREGAS_API UCoreGameplayAbility_AIMovement_EQS : public UCoreGameplayAbility_AIMovementBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	TObjectPtr<UEnvQuery> RepositionQuery;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	TEnumAsByte<EEnvQueryRunMode::Type> QueryRunMode = EEnvQueryRunMode::RandomBest25Pct;

private:
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);
};
