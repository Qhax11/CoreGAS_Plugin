// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "CoreStateBase.generated.h"

class UCoreStateManager;

USTRUCT()
struct FCoreStateContext
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY()
	TObjectPtr<UCoreASCBase> OwnerASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateTransitionRequested, FGameplayTag);

UCLASS(Abstract, Blueprintable, EditInlineNew)
class COREGAS_API UCoreStateBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (Categories = "CoreGAS.AI.State"))
	FGameplayTag StateTag;

	UPROPERTY()
	FCoreStateContext Context;

	FOnStateTransitionRequested OnStateTransitionRequested;

	void InitializeContext(const FCoreStateContext& InContext);

	virtual void OnEnter(UCoreStateManager* StateManager);
	virtual void OnExit(UCoreStateManager* StateManager);
	virtual void OnTick(UCoreStateManager* StateManager, float DeltaTime);
	virtual bool EnterCondition(UCoreStateManager* StateManager) const;

protected:
	void BroadcastTransition(FGameplayTag TargetStateTag);
};
