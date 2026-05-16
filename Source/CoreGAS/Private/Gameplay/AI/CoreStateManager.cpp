// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/States/CoreStateBase.h"
#include "Gameplay/Debug/CoreGameplayLog.h"

UCoreStateManager::UCoreStateManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoreStateManager::Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor, UCoreAIBehaviorDecision* BehaviorDecision)
{
	StateContext.OwnerActor       = OwnerActor;
	StateContext.OwnerASC         = OwnerASC;
	StateContext.TargetActor      = TargetActor;
	StateContext.BehaviorDecision = BehaviorDecision;

	StateInstances.Reset();

	for (TSubclassOf<UCoreStateBase> StateClass : StateClassArray)
	{
		if (!StateClass)
		{
			continue;
		}

		UCoreStateBase* NewState = NewObject<UCoreStateBase>(this, StateClass);
		NewState->InitializeContext(StateContext);
		StateInstances.Add(NewState);
	}

	StartLogic();
}

void UCoreStateManager::RequestStateEnter(FGameplayTag StateTag)
{
	for (UCoreStateBase* State : StateInstances)
	{
		if (!State || State->StateTag != StateTag)
		{
			continue;
		}

		if (!State->EnterCondition(this))
		{
			return;
		}

		if (CurrentState)
		{
			CurrentState->OnExit(this);
		}

		UE_LOG(LogCoreAI, Log, TEXT("[StateManager] Transition -> %s"), *StateTag.ToString());
		CurrentState = State;
		CurrentState->OnEnter(this);
		return;
	}
}

void UCoreStateManager::StartLogic()
{
	RequestStateEnter(StartState);
}

FGameplayTag UCoreStateManager::GetCurrentStateTag() const
{
	return CurrentState ? CurrentState->StateTag : FGameplayTag();
}

