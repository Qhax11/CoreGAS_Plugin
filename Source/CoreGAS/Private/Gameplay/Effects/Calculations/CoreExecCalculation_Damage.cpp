// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Effects/Calculations/CoreExecCalculation_Damage.h"
#include "Gameplay/Attributes/CoreAttributeSetBase.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "GameplayEffect.h"
#include "GameplayEffectAggregator.h"

void UCoreExecCalculation_Damage::ExecuteWithParams(FCoreExecCalculationParameters Params,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 1. Immunity check
	if (Params.TargetASC && Params.TargetASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Status_Invincible))
	{
		return;
	}

	const UCoreAttributeSetBase* TargetSet = Params.GetTargetAttributeSet();
	if (!TargetSet)
	{
		return;
	}

	// 2. Raw damage from SetByCaller
	const float RawDamage = GetRawDamage(Params);

	// 3-4. Armor mitigation
	const float Armor = TargetSet->GetArmor();
	const float MitigatedDamage = CalculateArmorMitigation(RawDamage, Armor);

	// 5-6. Final damage clamped to current health
	const float CurrentHealth = TargetSet->GetHealth();
	const float DamageDealt = CalculateFinalDamage(MitigatedDamage, CurrentHealth);

	// 7. Apply as negative modifier to Health
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UCoreAttributeSetBase::GetHealthAttribute(), EGameplayModOp::Additive, -DamageDealt));
}

float UCoreExecCalculation_Damage::GetRawDamage(FCoreExecCalculationParameters& Params) const
{
	return GetSetByCallerMagnitude(Params.GetSpec(), CoreGAS::Combat::TAG_SetByCaller_DamageAmount);
}

float UCoreExecCalculation_Damage::CalculateArmorMitigation(float RawDamage, float Armor) const
{
	return RawDamage * (1.f - (Armor / (Armor + 100.f)));
}

float UCoreExecCalculation_Damage::CalculateFinalDamage(float MitigatedDamage, float CurrentHealth) const
{
	return FMath::Clamp(MitigatedDamage, 0.f, CurrentHealth);
}
