// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Data/CoreGASData.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "GameplayEffect.h"
#include "Engine/DataTable.h"

UCoreASCBase* UCoreGASLibrary::GetCoreASCFromActor(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UCoreASCBase>() : nullptr;
}

void UCoreGASLibrary::ApplyGASData(UCoreASCBase* ASC, UAttributeSet* AttributeSet, UCoreGASData* Data)
{
	if (!ASC || !AttributeSet || !Data)
	{
		return;
	}

	// Initialize attribute set from DataTable if provided
	if (UDataTable* InitTable = Data->AttributeInitializationData.LoadSynchronous())
	{
		AttributeSet->InitFromMetaDataTable(InitTable);
	}

	auto GrantAbilities = [&](const TArray<TSubclassOf<UCoreGameplayAbilityBase>>& Abilities)
	{
		for (const auto& AbilityClass : Abilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1);
				if (const UCoreGameplayAbilityBase* CDO = AbilityClass->GetDefaultObject<UCoreGameplayAbilityBase>())
				{
					if (CDO->AbilityInputTag.IsValid())
					{
						UE_LOG(LogTemp, Warning, TEXT("UCoreGASLibrary: debug: tag name: %s"), *CDO->AbilityInputTag.ToString());
						Spec.DynamicAbilityTags.AddTag(CDO->AbilityInputTag);
					}
				}
				ASC->GiveAbility(Spec);
			}
		}
	};

	GrantAbilities(Data->AttackAbilities);
	GrantAbilities(Data->ComboAbilities);
	GrantAbilities(Data->ReactionAbilities);
	GrantAbilities(Data->MovementAbilities);
	GrantAbilities(Data->PassiveAbilities);
	GrantAbilities(Data->UtilityAbilities);

	// Apply startup effects
	for (const TSubclassOf<UGameplayEffect>& EffectClass : Data->StartupEffects)
	{
		if (EffectClass)
		{
			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			ASC->ApplyGameplayEffectToSelf(EffectCDO, 1.0f, ASC->MakeEffectContext());
		}
	}

	// Add permanent tags
	if (Data->PermanentTags.IsValid())
	{
		ASC->AddLooseGameplayTags(Data->PermanentTags);
	}
}

bool UCoreGASLibrary::GetCooldownRemainingAndDurationByContainer(
	UAbilitySystemComponent* ASC,
	FGameplayTagContainer CooldownTags,
	float& OutTimeRemaining,
	float& OutDuration)
{
	if (!ASC || !CooldownTags.IsValid())
	{
		OutTimeRemaining = 0.f;
		OutDuration = 0.f;
		return false;
	}

	OutTimeRemaining = 0.f;
	OutDuration = 0.f;

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	TArray<TPair<float, float>> RemainingAndDurations = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);

	for (const TPair<float, float>& Pair : RemainingAndDurations)
	{
		if (Pair.Key > OutTimeRemaining)
		{
			OutTimeRemaining = Pair.Key;
			OutDuration = Pair.Value;
		}
	}

	return OutTimeRemaining > 0.f;
}

bool UCoreGASLibrary::GetCooldownRemainingAndDurationByTag(
	UAbilitySystemComponent* ASC,
	FGameplayTag CooldownTag,
	float& OutTimeRemaining,
	float& OutDuration)
{
	if (!ASC || !CooldownTag.IsValid())
	{
		OutTimeRemaining = 0.f;
		OutDuration = 0.f;
		return false;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(CooldownTag);
	return GetCooldownRemainingAndDurationByContainer(ASC, TagContainer, OutTimeRemaining, OutDuration);
}
