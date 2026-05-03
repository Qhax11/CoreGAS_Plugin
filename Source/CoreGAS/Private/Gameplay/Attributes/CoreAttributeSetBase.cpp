// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Attributes/CoreAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreAttributeTags.h"

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

		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			if (GetHealth() >= GetMaxHealth())
			{
				ASC->AddLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Full);
				ASC->RemoveLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Empty, 1);
			}
			else if (GetHealth() <= 0.f)
			{
				ASC->AddLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Empty);
				ASC->RemoveLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Full, 1);
			}
			else
			{
				ASC->RemoveLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Full, 1);
				ASC->RemoveLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Empty, 1);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}

void UCoreAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMovementSpeedAttribute())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor()))
		{
			OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = NewValue;
		}
	}
}
