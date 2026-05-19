// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Debug/CoreGameplayLog.h"

void UCoreAttackDecisionService::Initialize(const FCoreDecisionServiceInitParams& Params)
{
	CachedOwner = Params.OwnerActor;
	CachedOwnerASC = Params.OwnerASC;
	CachedTarget = Params.TargetActor;
	CachedTargetASC = Params.TargetASC;
}

const FCoreAttackDataBase* UCoreAttackDecisionService::GetBestAttack(const TArray<TInstancedStruct<FCoreAttackDataBase>>& AttackOptions)
{
	TArray<const FCoreAttackDataBase*> Valid;
	const float Distance = GetDistanceToTarget();
	for (const TInstancedStruct<FCoreAttackDataBase>& Option : AttackOptions)
	{
		const FCoreAttackDataBase* Data = Option.GetPtr<FCoreAttackDataBase>();
		if (!Data) continue;
		if (Data->CooldownTag.IsValid() && CachedOwnerASC && CachedOwnerASC->HasMatchingGameplayTag(Data->CooldownTag)) continue;
		if (Distance < Data->PreferredMinDistance || Distance > Data->PreferredMaxDistance) continue;
		Valid.Add(Data);
		UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Candidate: %s | Distance: %.1f | Weight: %.2f"), *Data->AttackName.ToString(), Distance, Data->SelectionWeight);
	}
	if (Valid.IsEmpty()) return nullptr;

	float TotalWeight = 0.f;
	for (const FCoreAttackDataBase* Data : Valid)
		TotalWeight += Data->SelectionWeight;

	float Roll = FMath::FRandRange(0.f, TotalWeight);
	float Accumulated = 0.f;
	const FCoreAttackDataBase* Selected = Valid.Last();

	for (const FCoreAttackDataBase* Data : Valid)
	{
		Accumulated += Data->SelectionWeight;
		if (Roll <= Accumulated)
		{
			Selected = Data;
			break;
		}
	}

	UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Selected: %s (Roll: %.2f / Total: %.2f)"), *Selected->AttackName.ToString(), Roll, TotalWeight);
	return Selected;
}

float UCoreAttackDecisionService::GetDistanceToTarget() const
{
	if (!CachedOwner || !CachedTarget)
	{
		return MAX_FLT;
	}
	return FVector::Dist(CachedOwner->GetActorLocation(), CachedTarget->GetActorLocation());
}