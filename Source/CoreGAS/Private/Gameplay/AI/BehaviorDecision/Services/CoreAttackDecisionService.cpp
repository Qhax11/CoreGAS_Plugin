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
		UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Candidate: %s | Distance: %.1f"), *Data->AttackName.ToString(), Distance);
	}

	if (Valid.IsEmpty()) return nullptr;

	const FCoreAttackDataBase* Selected = Valid[FMath::RandRange(0, Valid.Num() - 1)];
	UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Selected: %s"), *Selected->AttackName.ToString());
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