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

	UPROPERTY()
	TObjectPtr<UCoreAttackDecisionService> AttackDecisionService;

	void Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor, UAbilitySystemComponent* TargetASC);
	void SetBehaviorData(UCoreEnemyAIData* ArchetypeData);
	const FCoreAttackDataBase* GetBestAttack();
	const FCoreAttackDataBase* GetSelectedAttack() const;
	FGameplayTag DecideNextState() const;
	float GetDistanceToTarget() const;

	UPROPERTY()
	TObjectPtr<UCoreEnemyAIData> CachedArchetypeData;

private:
	const FCoreAttackDataBase* SelectedAttack = nullptr;

	AActor* CachedOwnerActor = nullptr;
};
