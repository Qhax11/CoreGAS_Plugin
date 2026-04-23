// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Effects/Calculations/CoreExecCalculationBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectAggregator.h"

void UCoreExecCalculationBase::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FCoreExecCalculationParameters Params(ExecutionParams);
	ExecuteWithParams(Params, OutExecutionOutput);
}

void UCoreExecCalculationBase::MakeAggregatorEvaluateParams(const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters& OutParams) const
{
	OutParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	OutParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
}

float UCoreExecCalculationBase::GetSetByCallerMagnitude(const FGameplayEffectSpec& Spec, const FGameplayTag& DataTag, float Default, bool bWarn) const
{
	return Spec.GetSetByCallerMagnitude(DataTag, bWarn, Default);
}
