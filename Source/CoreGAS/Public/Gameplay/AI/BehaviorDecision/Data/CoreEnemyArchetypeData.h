// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackDataBase.h"
#include "CoreEnemyArchetypeData.generated.h"

UCLASS(BlueprintType)
class COREGAS_API UCoreEnemyArchetypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "CoreGAS|AI")
	TArray<TObjectPtr<UCoreAttackDataBase>> AttackOptions;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float AggroRange = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float AttackDecisionInterval = 1.5f;
};
