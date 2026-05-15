// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Debug/CoreGameplayLog.h"

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
		const FCoreAttackDataBase* Data = Option.GetPtr<FCoreAttackDataBase>();
		if (!Data)
		{
			continue;
		}

		if (Data->CooldownTag.IsValid() && CachedOwnerASC && CachedOwnerASC->HasMatchingGameplayTag(Data->CooldownTag))
		{
			continue;
		}

		float MinRange = 50.f;
		float MaxRange = 150.f;

		const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(Data);
		if (SimpleAttack && SimpleAttack->AbilityClass)
		{
			if (UCoreGameplayAbility_AttackBase* CDO = SimpleAttack->AbilityClass->GetDefaultObject<UCoreGameplayAbility_AttackBase>())
			{
				MinRange = CDO->MinRange;
				MaxRange = CDO->MaxRange;
			}
		}

		float Distance = 0.f;
		float DistanceScore = -1.f;
		if (CachedOwner && CachedTarget)
		{
			Distance = FVector::Dist(CachedOwner->GetActorLocation(), CachedTarget->GetActorLocation());
			if (Distance < MinRange)
			{
				DistanceScore = -1.f;
			}
			else if (Distance > MaxRange)
			{
				DistanceScore = -0.5f;
			}
			else
			{
				const float NormalizedDist = (Distance - MinRange) / (MaxRange - MinRange);
				DistanceScore = FMath::Lerp(-1.f, 1.f, NormalizedDist);
			}
		}

		const float TotalScore = DistanceScore + Data->ScoreBias;
		UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Attack: %s | Distance: %.1f | Score: %.2f"), *Data->AttackName.ToString(), Distance, TotalScore);
		if (TotalScore > BestScore)
		{
			BestScore = TotalScore;
			BestAttack = Data;
		}
	}

	return BestAttack;
}

float UCoreAttackDecisionService::GetDistanceToTarget() const
{
	if (!CachedOwner || !CachedTarget)
	{
		return MAX_FLT;
	}

	return FVector::Dist(CachedOwner->GetActorLocation(), CachedTarget->GetActorLocation());
}