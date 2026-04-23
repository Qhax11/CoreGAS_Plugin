// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Types/CoreGASTypes.h"

FCoreExecCalculationParameters::FCoreExecCalculationParameters(const FGameplayEffectCustomExecutionParameters& InParams)
{
	SourceASC = Cast<UCoreASCBase>(InParams.GetSourceAbilitySystemComponent());
	SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	TargetASC = Cast<UCoreASCBase>(InParams.GetTargetAbilitySystemComponent());
	TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = InParams.GetOwningSpec();
	if (Spec.Def)
	{
		EffectAssetTags = Spec.Def->InheritableGameplayEffectTags.CombinedTags;
	}

	MutableSpec = InParams.GetOwningSpecForPreExecuteMod();
	ExecutionParams = &InParams;
	HitResult = InParams.GetOwningSpec().GetContext().GetHitResult();
}

const UCoreAttributeSetBase* FCoreExecCalculationParameters::GetSourceAttributeSet() const
{
	return SourceASC ? SourceASC->GetSet<UCoreAttributeSetBase>() : nullptr;
}

const UCoreAttributeSetBase* FCoreExecCalculationParameters::GetTargetAttributeSet() const
{
	return TargetASC ? TargetASC->GetSet<UCoreAttributeSetBase>() : nullptr;
}

const FGameplayEffectSpec& FCoreExecCalculationParameters::GetSpec() const
{
	check(MutableSpec);
	return *MutableSpec;
}

const FHitResult* FCoreExecCalculationParameters::GetHitResult() const
{
	return HitResult;
}
