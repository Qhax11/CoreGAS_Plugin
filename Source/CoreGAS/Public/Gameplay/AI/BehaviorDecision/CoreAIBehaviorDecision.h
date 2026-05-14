// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/AI/BehaviorDecision/Services/CoreAttackDecisionService.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreEnemyArchetypeData.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "CoreAIBehaviorDecision.generated.h"

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreAIBehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoreAIBehaviorDecision();

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Debug")
	bool bEnableDebug = true;

	UPROPERTY()
	TObjectPtr<UCoreAttackDecisionService> AttackDecisionService;

	void Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor, UAbilitySystemComponent* TargetASC);
	void SetArchetypeData(UCoreEnemyArchetypeData* ArchetypeData);
	const FCoreAttackDataBase* GetBestAttack();
	const FCoreAttackDataBase* GetSelectedAttack() const;
	FGameplayTag DecideNextState() const;

	UPROPERTY()
	TObjectPtr<UCoreEnemyArchetypeData> CachedArchetypeData;

private:
	const FCoreAttackDataBase* SelectedAttack = nullptr;
};
