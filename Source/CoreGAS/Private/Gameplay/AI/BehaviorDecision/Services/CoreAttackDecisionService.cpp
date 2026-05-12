// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"

void UCoreAttackDecisionService::Initialize(const FCoreDecisionServiceInitParams& Params)
{
	CachedOwner     = Params.OwnerActor;
	CachedOwnerASC  = Params.OwnerASC;
	CachedTarget    = Params.TargetActor;
	CachedTargetASC = Params.TargetASC;
}

UCoreAttackDataBase* UCoreAttackDecisionService::GetBestAttack(const TArray<UCoreAttackDataBase*>& AttackOptions)
{
	FAttackDecisionContext Context;
	Context.Owner     = CachedOwner;
	Context.Target    = CachedTarget;
	Context.OwnerASC  = CachedOwnerASC;
	Context.TargetASC = CachedTargetASC;

	UCoreAttackDataBase* BestAttack = nullptr;
	float BestScore = -MAX_FLT;

	for (UCoreAttackDataBase* Attack : AttackOptions)
	{
		if (!Attack)
		{
			continue;
		}

		if (!Attack->IsEnable(Context))
		{
			continue;
		}

		if (!Attack->PassesChance(Context))
		{
			continue;
		}

		const float Score = Attack->GetScore(Context);
		if (Score > BestScore)
		{
			BestScore  = Score;
			BestAttack = Attack;
		}
	}

	return BestAttack;
}
