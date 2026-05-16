// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/States/CoreStateBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "CoreState_Movement.generated.h"

struct FCoreAttackDataBase;

UCLASS()
class COREGAS_API UCoreState_Movement : public UCoreStateBase
{
	GENERATED_BODY()

public:
	UCoreState_Movement();

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float AcceptanceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (Categories = "CoreGAS.AI.State"))
	FGameplayTag TransitionTag;

	virtual void OnEnter(UCoreStateManager* StateManager) override;
	virtual void OnExit(UCoreStateManager* StateManager) override;

private:
	FGameplayAbilitySpecHandle ActiveAbilityHandle;
	FDelegateHandle            EndListenerHandle;

	FGameplayEventData BuildMovementEventData(const FCoreAttackDataBase* BestAttack) const;
	void OnMovementAbilityEnded(const FAbilityEndedData& EndData);
};
