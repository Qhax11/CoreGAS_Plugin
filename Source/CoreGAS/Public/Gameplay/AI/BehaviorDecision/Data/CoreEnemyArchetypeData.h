// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "CoreEnemyArchetypeData.generated.h"

UCLASS(BlueprintType)
class COREGAS_API UCoreEnemyArchetypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FCoreAttackDataBase>> AttackOptions;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float AggroRange = 1500.f;
};
