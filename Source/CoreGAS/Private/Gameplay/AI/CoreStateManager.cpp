// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/States/CoreStateBase.h"
#include "DrawDebugHelpers.h"

UCoreStateManager::UCoreStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCoreStateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CurrentState)
	{
		return;
	}

	CurrentState->OnTick(this, DeltaTime);

	if (bEnableDebug)
	{
		const FString TagFull  = CurrentState->StateTag.ToString();
		int32 LastDotIndex;
		const FString TagLabel = TagFull.FindLastChar(TEXT('.'), LastDotIndex)
			? TagFull.RightChop(LastDotIndex + 1)
			: TagFull;

		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 150.f), TagLabel, nullptr, FColor::White, 0.f, true);
	}
}

void UCoreStateManager::CreateStates()
{
	StateInstances.Reset();

	for (TSubclassOf<UCoreStateBase> StateClass : StateClassArray)
	{
		if (!StateClass)
		{
			continue;
		}

		UCoreStateBase* NewState = NewObject<UCoreStateBase>(this, StateClass);
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
			CurrentState->OnStateTransitionRequested.RemoveAll(this);
			CurrentState->OnExit(this);
		}

		CurrentState = State;
		CurrentState->OnStateTransitionRequested.AddUObject(this, &UCoreStateManager::HandleStateTransitionRequested);
		CurrentState->OnEnter(this);
		return;
	}
}

void UCoreStateManager::StartLogic()
{
	RequestStateEnter(StartState);
}

void UCoreStateManager::HandleStateTransitionRequested(FGameplayTag TargetStateTag)
{
	RequestStateEnter(TargetStateTag);
}
