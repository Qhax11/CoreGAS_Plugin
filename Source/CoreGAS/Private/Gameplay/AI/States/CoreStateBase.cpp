// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreStateBase.h"

void UCoreStateBase::InitializeContext(const FCoreStateContext& InContext)
{
	Context = InContext;
}

void UCoreStateBase::OnEnter(UCoreStateManager* StateManager)
{
	CachedStateManager = StateManager;
}

void UCoreStateBase::OnExit(UCoreStateManager* StateManager)
{
	CachedStateManager = nullptr;
}

void UCoreStateBase::OnTick(UCoreStateManager* StateManager, float DeltaTime)
{
}

bool UCoreStateBase::EnterCondition(UCoreStateManager* StateManager) const
{
	return true;
}

void UCoreStateBase::RequestTransition(FGameplayTag TargetTag)
{
	if (CachedStateManager)
	{
		CachedStateManager->RequestStateEnter(TargetTag);
	}
}
