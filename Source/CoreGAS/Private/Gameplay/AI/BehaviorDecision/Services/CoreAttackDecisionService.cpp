// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Utilities/Combat/CoreCombatDistance.h"
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
	const float Distance = CoreCombat::GetDistance(CachedOwner, CachedTarget);

	for (const TInstancedStruct<FCoreAttackDataBase>& Option : AttackOptions)
	{
		const FCoreAttackDataBase* Data = Option.GetPtr<FCoreAttackDataBase>();
		if (!Data)
		{
			continue;
		}

		if (!IsAttackSelectable(Data, Distance))
		{
			continue;
		}

		Valid.Add(Data);
		UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Candidate: %s | Distance: %.1f | Weight: %.2f"), *Data->AttackName.ToString(), Distance, Data->SelectionConfig.SelectionWeight);
	}

	if (Valid.IsEmpty())
	{
		return nullptr;
	}

	float TotalWeight = 0.f;
	for (const FCoreAttackDataBase* Data : Valid) 
	{
		TotalWeight += Data->SelectionConfig.SelectionWeight;
	}

	float Roll = FMath::FRandRange(0.f, TotalWeight);
	float Accumulated = 0.f;
	const FCoreAttackDataBase* Selected = Valid.Last();

	for (const FCoreAttackDataBase* Data : Valid)
	{
		Accumulated += Data->SelectionConfig.SelectionWeight;
		if (Roll <= Accumulated)
		{
			Selected = Data;
			break;
		}
	}

	UE_LOG(LogCoreAIDecision, Verbose, TEXT("[AttackDecision] Selected: %s (Roll: %.2f / Total: %.2f)"), *Selected->AttackName.ToString(), Roll, TotalWeight);
	return Selected;
}

bool UCoreAttackDecisionService::IsAttackSelectable(const FCoreAttackDataBase* Data, float Distance) const
{
	if (!Data)
	{
		return false;
	}

	if (!Data->AttackAbilityClass || !CachedOwnerASC)
	{
		return false;
	}

	if (Distance < Data->SelectionConfig.SelectionMinDistance || Distance > Data->SelectionConfig.SelectionMaxDistance)
	{
		return false;
	}

	const UGameplayAbility* CDO = Data->AttackAbilityClass->GetDefaultObject<UGameplayAbility>();
	const FGameplayTagContainer* CooldownTags = CDO ? CDO->GetCooldownTags() : nullptr;
	if (CooldownTags && !CooldownTags->IsEmpty() && CachedOwnerASC->HasAnyMatchingGameplayTags(*CooldownTags))
	{
		return false;
	}

	return true;
}

