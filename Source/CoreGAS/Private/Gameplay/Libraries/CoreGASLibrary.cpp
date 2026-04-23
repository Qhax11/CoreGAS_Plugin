// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "AttributeSet.h"
#include "Gameplay/Data/CoreCharacterData.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "GameplayEffect.h"
#include "Engine/DataTable.h"

UCoreASCBase* UCoreGASLibrary::GetCoreASCFromActor(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UCoreASCBase>() : nullptr;
}

void UCoreGASLibrary::ApplyCharacterData(UCoreASCBase* ASC, UAttributeSet* AttributeSet, UCoreCharacterData* Data)
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

	// Grant startup abilities
	for (const TSubclassOf<UCoreGameplayAbilityBase>& AbilityClass : Data->StartupAbilities)
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
