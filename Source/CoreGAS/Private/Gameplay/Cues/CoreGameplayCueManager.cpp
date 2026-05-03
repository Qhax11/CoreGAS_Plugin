// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Cues/CoreGameplayCueManager.h"

void UCoreGameplayCueManager::InvokeGameplayCueExecuted_FromSpec(UAbilitySystemComponent* OwningComponent, const FGameplayEffectSpec& Spec, FPredictionKey PredictionKey)
{
	Super::InvokeGameplayCueExecuted_FromSpec(OwningComponent, Spec, PredictionKey);
	// Further customizations will be added here.
}

void UCoreGameplayCueManager::InvokeGameplayCueExecuted(UAbilitySystemComponent* OwningComponent, const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext)
{
	Super::InvokeGameplayCueExecuted(OwningComponent, GameplayCueTag, PredictionKey, EffectContext);
	// Further customizations will be added here.
}

void UCoreGameplayCueManager::InvokeGameplayCueExecuted_WithParams(UAbilitySystemComponent* OwningComponent, const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters)
{
	Super::InvokeGameplayCueExecuted_WithParams(OwningComponent, GameplayCueTag, PredictionKey, GameplayCueParameters);
	// Further customizations will be added here.
}
