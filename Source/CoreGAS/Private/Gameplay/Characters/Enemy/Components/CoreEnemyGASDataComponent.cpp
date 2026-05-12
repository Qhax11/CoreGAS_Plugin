// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Enemy/Components/CoreEnemyGASDataComponent.h"
#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"

void UCoreEnemyGASDataComponent::InitializeGASData(UCoreASCBase* ASC, UAttributeSet* AttributeSet, UCoreCharacterData* InCharacterData)
{
	UCoreCharacterData* DataToUse = InCharacterData ? InCharacterData : CharacterData.Get();
	UCoreGASLibrary::ApplyCharacterData(ASC, AttributeSet, DataToUse);

	if (ASC)
	{
		ASC->NotifyGASDataInitialized();
	}
}
