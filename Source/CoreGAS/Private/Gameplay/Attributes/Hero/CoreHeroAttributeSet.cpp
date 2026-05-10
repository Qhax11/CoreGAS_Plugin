// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Attributes/Hero/CoreHeroAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Tags/CoreCombatTags.h"

void UCoreHeroAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (AActor* OwnerActor = GetOwningActor())
		{
			FGameplayEventData EventData;
			EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();

			if (GetHealth() <= 0.f)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, CoreGAS::Combat::TAG_Event_Death, EventData);
			}
			else
			{
				EventData.EventMagnitude = Data.EvaluatedData.Magnitude;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, CoreGAS::Combat::TAG_Event_HitReaction, EventData);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}
