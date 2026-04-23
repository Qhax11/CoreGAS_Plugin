// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effects/Calculations/CoreExecCalculationBase.h"
#include "CoreExecCalculation_Damage.generated.h"

UCLASS()
class COREGAS_API UCoreExecCalculation_Damage : public UCoreExecCalculationBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteWithParams(FCoreExecCalculationParameters Params,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	virtual float GetRawDamage(FCoreExecCalculationParameters& Params) const;
	virtual float CalculateArmorMitigation(float RawDamage, float Armor) const;
	virtual float CalculateFinalDamage(float MitigatedDamage, float CurrentHealth) const;
};
