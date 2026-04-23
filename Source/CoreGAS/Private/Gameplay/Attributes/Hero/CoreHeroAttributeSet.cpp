// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Attributes/Hero/CoreHeroAttributeSet.h"
#include "GameplayEffectExtension.h"

void UCoreHeroAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}
