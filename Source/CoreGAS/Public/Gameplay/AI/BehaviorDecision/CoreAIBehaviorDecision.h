// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"
#include "Gameplay/Data/CoreEnemyAIData.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "CoreAIBehaviorDecision.generated.h"

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreAIBehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoreAIBehaviorDecision();

	void Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor, UAbilitySystemComponent* TargetASC);
	void SetBehaviorData(UCoreEnemyAIData* AIBehaviorData);
	const FCoreAttackDataBase* GetBestAttack();
	const FCoreAttackDataBase* GetSelectedAttack() const;
	FGameplayTag DecideNextState();
	float GetDistanceToTarget() const;

	UPROPERTY()
	TObjectPtr<UCoreEnemyAIData> AIData;

private:
	UPROPERTY()
	TObjectPtr<UCoreAttackDecisionService> AttackDecisionService;

	UPROPERTY()
	const FCoreAttackDataBase* SelectedAttack = nullptr;

	UPROPERTY()
	AActor* CachedOwnerActor = nullptr;
};
