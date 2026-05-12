// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreDecisionOptionData.h"
#include "CoreAttackDataBase.generated.h"

USTRUCT()
struct FAttackDecisionContext
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> Target = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
};

UENUM()
enum class EAttackDisableReason : uint8
{
	None,
	InvalidContext,
	OnCooldown,
	RuleBlocked,
	InvalidAbility
};

USTRUCT()
struct FAttackEnableDebug
{
	GENERATED_BODY()

	UPROPERTY()
	EAttackDisableReason Reason = EAttackDisableReason::None;
};

USTRUCT()
struct FAttackScoreDebug
{
	GENERATED_BODY()

	UPROPERTY()
	float DistanceScore = 0.f;

	UPROPERTY()
	float BiasScore = 0.f;

	UPROPERTY()
	float TotalScore = 0.f;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class COREGAS_API UCoreAttackDataBase : public UCoreDecisionOptionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	FName AttackName;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float MinRange = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float MaxRange = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float ScoreBias = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (Categories = "Ability.Cooldown"))
	FGameplayTag CooldownTag;

	virtual bool IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug = nullptr) const;
	virtual bool PassesChance(const FAttackDecisionContext& Context) const { return true; }
	virtual float GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug = nullptr) const;

protected:
	float GetDistanceScore(const FAttackDecisionContext& Context) const;
};
