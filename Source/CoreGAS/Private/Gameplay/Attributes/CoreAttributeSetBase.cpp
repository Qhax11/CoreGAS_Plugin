// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Attributes/CoreAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Tags/CoreAttributeTags.h"
#include "Gameplay/Tags/CoreCombatTags.h"

UCoreAttributeSetBase::UCoreAttributeSetBase()
{

}

void UCoreAttributeSetBase::InitFromMetaDataTable(const UDataTable* DataTable)
{
	Super::InitFromMetaDataTable(DataTable);

	UCoreASCBase* ASC = Cast<UCoreASCBase>(GetOwningAbilitySystemComponent());
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAttributeSetBase::InitFromMetaDataTable: Failed to cast ASC to UCoreASCBase"));
		return;
	}

	ASC->OnGASDataInitialized.AddDynamic(this, &UCoreAttributeSetBase::InitializeAttributes);
}

void UCoreAttributeSetBase::InitializeAttributes()
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	if (GetHealth() >= GetMaxHealth())
	{
		ASC->AddLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Full);
		ASC->RemoveLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Empty, 1);
	}
	else
	{
		ASC->RemoveLooseGameplayTag(CoreGAS::Attribute::TAG_Attribute_Health_Full, 1);
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor()))
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = GetMovementSpeed();
	}
}

void UCoreAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UCoreAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (AActor* OwnerActor = GetOwningActor())
		{
			FGameplayEventData EventData;
			EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();

			EventData.EventMagnitude = Data.EvaluatedData.Magnitude;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, CoreGAS::Combat::TAG_Event_HitReaction, EventData);
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

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
