// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/States/CoreStateBase.h"
#include "GameplayAbilitySpec.h"
#include "CoreState_AbilityActivation.generated.h"

class UGameplayAbility;

UCLASS()
class COREGAS_API UCoreState_AbilityActivation : public UCoreStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	FGameplayTag TransitionTag;

	virtual void OnEnter(UCoreStateManager* StateManager) override;
	virtual void OnExit(UCoreStateManager* StateManager) override;

protected:
	virtual void OnAbilityEnded();

private:

	FGameplayAbilitySpecHandle ActiveAbilityHandle;
	FDelegateHandle            EndListenerHandle;
};
