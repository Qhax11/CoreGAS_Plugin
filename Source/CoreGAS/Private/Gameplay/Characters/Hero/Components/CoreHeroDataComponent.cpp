// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Hero/Components/CoreHeroDataComponent.h"
#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Data/CoreHeroGASData.h"

void UCoreHeroDataComponent::Initialize(UCoreASCBase* ASC, UAttributeSet* AttributeSet)
{
	UCoreGASLibrary::ApplyGASData(ASC, AttributeSet, GASData);

	if (ASC)
	{
		ASC->NotifyGASDataInitialized();
	}
}
