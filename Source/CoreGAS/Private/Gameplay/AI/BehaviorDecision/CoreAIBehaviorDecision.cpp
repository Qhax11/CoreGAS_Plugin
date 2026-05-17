// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Debug/CoreGameplayLog.h"
#include "Gameplay/Tags/CoreAITags.h"

UCoreAIBehaviorDecision::UCoreAIBehaviorDecision()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoreAIBehaviorDecision::Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor, UAbilitySystemComponent* TargetASC)
{
	CachedOwnerActor = OwnerActor;

	AttackDecisionService = NewObject<UCoreAttackDecisionService>(this);

	FCoreDecisionServiceInitParams Params;
	Params.OwnerActor  = OwnerActor;
	Params.OwnerASC    = OwnerASC;
	Params.TargetActor = TargetActor;
	Params.TargetASC   = TargetASC;

	AttackDecisionService->Initialize(Params);
}

void UCoreAIBehaviorDecision::SetBehaviorData(UCoreEnemyAIData* AIBehaviorData)
{
	AIData = AIBehaviorData;
}

const FCoreAttackDataBase* UCoreAIBehaviorDecision::GetBestAttack()
{
	if (!AttackDecisionService || !AIData)
	{
		return nullptr;
	}

	const FCoreAttackDataBase* Result = AttackDecisionService->GetBestAttack(AIData->AttackOptions);
	SelectedAttack = Result;

	UE_LOG(LogCoreAIDecision, Log, TEXT("[BehaviorDecision] GetBestAttack: %s"), Result ? *Result->AttackName.ToString() : TEXT("null"));

	return Result;
}

const FCoreAttackDataBase* UCoreAIBehaviorDecision::GetSelectedAttack() const
{
	return SelectedAttack;
}

float UCoreAIBehaviorDecision::GetDistanceToTarget() const
{
	return AttackDecisionService ? AttackDecisionService->GetDistanceToTarget() : MAX_FLT;
}

FGameplayTag UCoreAIBehaviorDecision::DecideNextState() 
{
	FGameplayTag DecidedTag;

	if (!AIData || !AttackDecisionService)
	{
		DecidedTag = CoreGAS::AI::TAG_State_Idle;
	}
	else
	{
		const FCoreAttackDataBase* BestAttack = GetBestAttack();
		if (!BestAttack)
		{
			DecidedTag = CoreGAS::AI::TAG_State_Combat_Movement;
		}
		else
		{
			float MaxRange = 150.f;
			const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(BestAttack);
			if (SimpleAttack && SimpleAttack->AttackAbilityClass)
			{
				if (UCoreGameplayAbility_AttackBase* CDO = SimpleAttack->AttackAbilityClass->GetDefaultObject<UCoreGameplayAbility_AttackBase>())
				{
					MaxRange = CDO->MaxRange;
				}
			}

			DecidedTag = (AttackDecisionService->GetDistanceToTarget() > MaxRange)
				? CoreGAS::AI::TAG_State_Combat_Movement
				: CoreGAS::AI::TAG_State_Combat_Attack;
		}
	}

	UE_LOG(LogCoreAIDecision, Log, TEXT("[BehaviorDecision] DecideNextState: %s"), *DecidedTag.ToString());
	return DecidedTag;
}
