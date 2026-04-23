// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Attributes/CoreAttributeSetBase.h"
#include "GameplayEffectExtension.h"

UCoreAttributeSetBase::UCoreAttributeSetBase()
{

}

void UCoreAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}
