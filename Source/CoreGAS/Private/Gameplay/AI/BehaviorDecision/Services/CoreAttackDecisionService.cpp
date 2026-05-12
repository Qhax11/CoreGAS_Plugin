// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"

void UCoreAttackDecisionService::Initialize(const FCoreDecisionServiceInitParams& Params)
{
	CachedOwner     = Params.OwnerActor;
	CachedOwnerASC  = Params.OwnerASC;
	CachedTarget    = Params.TargetActor;
	CachedTargetASC = Params.TargetASC;
}

const FCoreAttackDataBase* UCoreAttackDecisionService::GetBestAttack(const TArray<TInstancedStruct<FCoreAttackDataBase>>& AttackOptions)
{
	const FCoreAttackDataBase* BestAttack = nullptr;
	float BestScore = -MAX_FLT;

	for (const TInstancedStruct<FCoreAttackDataBase>& Option : AttackOptions)
	{
		const FCoreAttackDataBase* Data = Option.GetPtr();
		if (!Data)
		{
			continue;
		}

		if (Data->CooldownTag.IsValid() && CachedOwnerASC && CachedOwnerASC->HasMatchingGameplayTag(Data->CooldownTag))
		{
			continue;
		}

		float DistanceScore = -1.f;
		if (CachedOwner && CachedTarget)
		{
			const float Distance = FVector::Dist(CachedOwner->GetActorLocation(), CachedTarget->GetActorLocation());

			if (Distance < Data->MinRange)
			{
				DistanceScore = -1.f;
			}
			else if (Distance > Data->MaxRange)
			{
				DistanceScore = -0.5f;
			}
			else
			{
				const float NormalizedDist = (Distance - Data->MinRange) / (Data->MaxRange - Data->MinRange);
				DistanceScore = FMath::Lerp(-1.f, 1.f, NormalizedDist);
			}
		}

		const float TotalScore = DistanceScore + Data->ScoreBias;
		if (TotalScore > BestScore)
		{
			BestScore  = TotalScore;
			BestAttack = Data;
		}
	}

	return BestAttack;
}
