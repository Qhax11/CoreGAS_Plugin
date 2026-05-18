// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/States/CoreState_AbilityActivation.h"
#include "CoreState_Attack.generated.h"

UCLASS()
class COREGAS_API UCoreState_Attack : public UCoreState_AbilityActivation
{
	GENERATED_BODY()

public:
	UCoreState_Attack();

	virtual void OnEnter(UCoreStateManager* StateManager) override;
	virtual void OnExit(UCoreStateManager* StateManager) override;

protected:
	virtual void OnAbilityEnded() override;
};
