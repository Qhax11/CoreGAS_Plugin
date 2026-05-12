// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackDataBase.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "CoreAttackDecisionService.generated.h"

USTRUCT()
struct FCoreDecisionServiceInitParams
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY()
	TObjectPtr<UCoreASCBase> OwnerASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
};

UCLASS()
class COREGAS_API UCoreAttackDecisionService : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(const FCoreDecisionServiceInitParams& Params);
	UCoreAttackDataBase* GetBestAttack(const TArray<UCoreAttackDataBase*>& AttackOptions);

protected:
	UPROPERTY()
	TObjectPtr<AActor> CachedOwner;

	UPROPERTY()
	TObjectPtr<UCoreASCBase> CachedOwnerASC;

	UPROPERTY()
	TObjectPtr<AActor> CachedTarget;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CachedTargetASC;
};
