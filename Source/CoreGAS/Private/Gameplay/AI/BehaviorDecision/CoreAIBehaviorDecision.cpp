// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"

UCoreAIBehaviorDecision::UCoreAIBehaviorDecision()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoreAIBehaviorDecision::Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor, UAbilitySystemComponent* TargetASC)
{
	AttackDecisionService = NewObject<UCoreAttackDecisionService>(this);

	FCoreDecisionServiceInitParams Params;
	Params.OwnerActor  = OwnerActor;
	Params.OwnerASC    = OwnerASC;
	Params.TargetActor = TargetActor;
	Params.TargetASC   = TargetASC;

	AttackDecisionService->Initialize(Params);
}

void UCoreAIBehaviorDecision::SetArchetypeData(UCoreEnemyArchetypeData* ArchetypeData)
{
	CachedArchetypeData = ArchetypeData;
}

const FCoreAttackDataBase* UCoreAIBehaviorDecision::GetBestAttack()
{
	if (!AttackDecisionService || !CachedArchetypeData)
	{
		return nullptr;
	}

	const FCoreAttackDataBase* Result = AttackDecisionService->GetBestAttack(CachedArchetypeData->AttackOptions);
	SelectedAttack = Result;

	if (bEnableDebug && Result)
	{
		UE_LOG(LogTemp, Log, TEXT("[BehaviorDecision] Best attack: %s"), *Result->AttackName.ToString());
	}

	return Result;
}

const FCoreAttackDataBase* UCoreAIBehaviorDecision::GetSelectedAttack() const
{
	return SelectedAttack;
}
