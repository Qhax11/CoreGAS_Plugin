// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Attributes/CoreAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Tags/CoreCombatTags.h"

UCoreAttributeSetBase::UCoreAttributeSetBase()
{

}

void UCoreAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (GetHealth() <= 0.f)
		{
			if (AActor* OwnerActor = GetOwningActor())
			{
				FGameplayEventData EventData;
				EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, CoreGAS::Combat::TAG_Event_Death, EventData);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}
