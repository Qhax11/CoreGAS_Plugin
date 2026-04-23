// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "CoreExecCalculationBase.generated.h"

UCLASS(Abstract)
class COREGAS_API UCoreExecCalculationBase : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const final override;

	virtual void ExecuteWithParams(FCoreExecCalculationParameters Params,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
	{
		UE_LOG(LogTemp, Error, TEXT("ExecuteWithParams not implemented in %s"), *GetName());
	}

protected:
	void MakeAggregatorEvaluateParams(const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters& OutParams) const;
	float GetSetByCallerMagnitude(const FGameplayEffectSpec& Spec, const FGameplayTag& DataTag, float Default = 0.f, bool bWarn = false) const;
};
