// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreStateBase.generated.h"

class UCoreStateManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateTransitionRequested, FGameplayTag);

UCLASS(Abstract, Blueprintable, EditInlineNew)
class COREGAS_API UCoreStateBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (Categories = "CoreGAS.AI.State"))
	FGameplayTag StateTag;

	FOnStateTransitionRequested OnStateTransitionRequested;

	virtual void OnEnter(UCoreStateManager* StateManager);
	virtual void OnExit(UCoreStateManager* StateManager);
	virtual void OnTick(UCoreStateManager* StateManager, float DeltaTime);
	virtual bool EnterCondition(UCoreStateManager* StateManager) const;

protected:
	void BroadcastTransition(FGameplayTag TargetStateTag);
};
