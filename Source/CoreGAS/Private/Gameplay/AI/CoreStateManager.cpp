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
		const FString TagFull = CurrentState->StateTag.ToString();
		int32 LastDotIndex;
		const FString TagLabel = TagFull.FindLastChar(TEXT('.'), LastDotIndex)
			? TagFull.RightChop(LastDotIndex + 1)
			: TagFull;

		AActor* OwnerActor = StateContext.OwnerActor;
		const FVector Location = OwnerActor ? OwnerActor->GetActorLocation() + FVector(0.f, 0.f, 150.f) : FVector::ZeroVector;
		DrawDebugString(GetWorld(), Location, TagLabel, nullptr, FColor::White, 0.f, true);
	}
}

void UCoreStateManager::Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor)
{
	StateContext.OwnerActor  = OwnerActor;
	StateContext.OwnerASC    = OwnerASC;
	StateContext.TargetActor = TargetActor;

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

		CurrentState = State;
		CurrentState->OnEnter(this);
		return;
	}
}

void UCoreStateManager::StartLogic()
{
	RequestStateEnter(StartState);
}

