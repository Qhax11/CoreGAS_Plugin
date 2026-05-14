// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Enemy/Components/CoreEnemyDataComponent.h"
#include "Gameplay/Data/CoreEnemyAIData.h"
#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/Libraries/CoreGASLibrary.h"
#include "Gameplay/Components/CoreASCBase.h"

void UCoreEnemyDataComponent::Initialize(UCoreASCBase* ASC, UAttributeSet* AttributeSet)
{
	if (!GASData || !AIData)
	{
		return;
	}

	AAIController* AIC = GetOwner() ? Cast<AAIController>(GetOwner()->GetInstigatorController()) : nullptr;
	ACoreAIController* CoreAIC = Cast<ACoreAIController>(AIC);

	CoreAIC->GetBehaviorDecision()->SetBehaviorData(AIData);
	UCoreGASLibrary::ApplyGASData(ASC, AttributeSet, GASData);

	if (ASC)
	{
		ASC->NotifyGASDataInitialized();
	}
}
