// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Tags/CoreAITags.h"

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

FGameplayTag UCoreAIBehaviorDecision::DecideNextState() const
{
	if (!CachedArchetypeData || !AttackDecisionService)
	{
		return CoreGAS::AI::TAG_State_Idle;
	}

	const FCoreAttackDataBase* BestAttack = AttackDecisionService->GetBestAttack(CachedArchetypeData->AttackOptions);
	if (!BestAttack)
	{
		return CoreGAS::AI::TAG_State_Combat_Movement;
	}

	float MaxRange = 150.f;
	const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(BestAttack);
	if (SimpleAttack && SimpleAttack->AbilityClass)
	{
		if (UCoreGameplayAbility_AttackBase* CDO = SimpleAttack->AbilityClass->GetDefaultObject<UCoreGameplayAbility_AttackBase>())
		{
			MaxRange = CDO->MaxRange;
		}
	}

	if (AttackDecisionService->GetDistanceToTarget() > MaxRange)
	{
		return CoreGAS::AI::TAG_State_Combat_Movement;
	}

	return CoreGAS::AI::TAG_State_Combat_Attack;
}
