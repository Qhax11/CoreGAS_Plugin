// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackDataBase.h"

bool UCoreAttackDataBase::IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug) const
{
	if (!Context.Target)
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::InvalidContext;
		}
		return false;
	}

	if (CooldownTag.IsValid() && Context.OwnerASC && Context.OwnerASC->HasMatchingGameplayTag(CooldownTag))
	{
		if (OutDebug)
		{
			OutDebug->Reason = EAttackDisableReason::OnCooldown;
		}
		return false;
	}

	return true;
}

float UCoreAttackDataBase::GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug) const
{
	const float DistanceScore = GetDistanceScore(Context);
	const float TotalScore = DistanceScore + ScoreBias;

	if (OutDebug)
	{
		OutDebug->DistanceScore = DistanceScore;
		OutDebug->BiasScore = ScoreBias;
		OutDebug->TotalScore = TotalScore;
	}

	return TotalScore;
}

float UCoreAttackDataBase::GetDistanceScore(const FAttackDecisionContext& Context) const
{
	if (!Context.Owner || !Context.Target)
	{
		return -1.f;
	}

	const float Distance = FVector::Dist(Context.Owner->GetActorLocation(), Context.Target->GetActorLocation());

	if (Distance < MinRange)
	{
		return -1.f;
	}

	if (Distance > MaxRange)
	{
		return -0.5f;
	}

	const float NormalizedDist = (Distance - MinRange) / (MaxRange - MinRange);
	return FMath::Lerp(-1.f, 1.f, NormalizedDist);
}
