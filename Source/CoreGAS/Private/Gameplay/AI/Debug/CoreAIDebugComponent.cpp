// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/Debug/CoreAIDebugComponent.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"

UCoreAIDebugComponent::UCoreAIDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCoreAIDebugComponent::Initialize(AActor* OwnerActor, UCoreStateManager* InStateManager, UCoreAIBehaviorDecision* InBehaviorDecision)
{
	ChacedOwner = OwnerActor;
	StateManager = InStateManager;
	BehaviorDecision = InBehaviorDecision;
}

void UCoreAIDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnableDebug)
	{
		return;
	}

	if (!ChacedOwner || !StateManager || !BehaviorDecision) 
	{
		return;
	}

	UWorld* World = GetWorld();
	const FVector BaseLocation = ChacedOwner->GetActorLocation() + FVector(0.f, 0.f, 200.f);

	FString StateLabel = TEXT("None");
	const FGameplayTag StateTag = StateManager->GetCurrentStateTag();
	if (StateTag.IsValid())
	{
		const FString TagFull = StateTag.ToString();
		int32 LastDotIndex;
		StateLabel = TagFull.FindLastChar(TEXT('.'), LastDotIndex)
			? TagFull.RightChop(LastDotIndex + 1)
			: TagFull;
	}
	DrawDebugString(World, BaseLocation, FString::Printf(TEXT("State: %s"), *StateLabel), nullptr, FColor::White, 0.f, true);

	FString AttackLabel = TEXT("None");
	if (const FCoreAttackDataBase* SelectedAttack = BehaviorDecision->GetSelectedAttack())
	{
		AttackLabel = SelectedAttack->AttackName.ToString();
	}
	DrawDebugString(World, BaseLocation + FVector(0.f, 0.f, 25.f), FString::Printf(TEXT("Attack: %s"), *AttackLabel), nullptr, FColor::Yellow, 0.f, true);

	FString DistLabel = TEXT("N/A");
	const float Dist = BehaviorDecision->GetDistanceToTarget();
	if (Dist < MAX_FLT)
	{
		DistLabel = FString::Printf(TEXT("%.0f"), Dist);
	}
	DrawDebugString(World, BaseLocation + FVector(0.f, 0.f, 50.f), FString::Printf(TEXT("Dist: %s"), *DistLabel), nullptr, FColor::Cyan, 0.f, true);
}
