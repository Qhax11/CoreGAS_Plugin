// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreStateBase.h"
#include "Gameplay/Debug/CoreGameplayLog.h"

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
    if (!CachedStateManager)
    {
        CORE_AI_LOG(LogCoreAI, Warning, "RequestTransition FAILED — CachedStateManager is NULL | From: %s | To: %s",
            *StateTag.ToString(), *TargetTag.ToString());
        return;
    }

    if (!TargetTag.IsValid())
    {
        CORE_AI_LOG(LogCoreAI, Warning, "RequestTransition FAILED — TargetTag is INVALID | From: %s",
            *StateTag.ToString());
        return;
    }

    CORE_AI_LOG(LogCoreAI, Log, "RequestTransition — From: %s | To: %s",
        *StateTag.ToString(), *TargetTag.ToString());

    CachedStateManager->RequestStateEnter(TargetTag);
}
