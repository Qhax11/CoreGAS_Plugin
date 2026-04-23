// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Hero/Components/CoreHeroGASDataComponent.h"
#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Data/CoreHeroData.h"

void UCoreHeroGASDataComponent::InitializeGASData(UCoreASCBase* ASC, UAttributeSet* AttributeSet)
{
	UCoreGASLibrary::ApplyCharacterData(ASC, AttributeSet, HeroData);

	if (ASC)
	{
		ASC->NotifyGASDataInitialized();
	}
}
