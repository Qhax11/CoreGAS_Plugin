// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/Debug/CoreAIDebugComponent.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Utilities/Combat/CoreCombatDistance.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"

UCoreAIDebugComponent::UCoreAIDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCoreAIDebugComponent::Initialize(AActor* OwnerActor, AActor* TargetActor, UCoreStateManager* InStateManager, UCoreAIBehaviorDecision* InBehaviorDecision)
{
    CachedOwner = OwnerActor;
    CachedTarget = TargetActor;
	StateManager = InStateManager;
	BehaviorDecision = InBehaviorDecision;
}

void UCoreAIDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnableDebug || !CachedOwner || !StateManager || !BehaviorDecision)
		return;

	const FVector BaseLocation = CachedOwner->GetActorLocation() + FVector(0.f, 0.f, 200.f);
	DrawState(BaseLocation);
	DrawAttack(BaseLocation);
	DrawDistance(BaseLocation);
}

void UCoreAIDebugComponent::DrawState(const FVector& BaseLocation) const
{
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

    if (StateTag == CoreGAS::AI::TAG_State_Combat_Movement)
    {
        if (const FCoreAttackDataBase* SelectedAttack = BehaviorDecision->GetSelectedAttack())
        {
            if (const FCoreMovementConfigBase* Config = SelectedAttack->MovementConfig.GetPtr<FCoreMovementConfigBase>())
            {
                FString ConfigName = Config->GetScriptStruct()->GetName();
                ConfigName.RemoveFromStart(TEXT("CoreMovementConfig_"));
                StateLabel += TEXT(", ") + ConfigName;
            }
        }
    }

    DrawDebugString(GetWorld(), BaseLocation, FString::Printf(TEXT("State: %s"), *StateLabel), nullptr, FColor::White, 0.f, true);
}

void UCoreAIDebugComponent::DrawAttack(const FVector& BaseLocation) const
{
	FString AttackLabel = TEXT("None");
	if (const FCoreAttackDataBase* SelectedAttack = BehaviorDecision->GetSelectedAttack())
		AttackLabel = SelectedAttack->AttackName.ToString();

	DrawDebugString(GetWorld(), BaseLocation + FVector(0.f, 0.f, 25.f), FString::Printf(TEXT("Selected Attack: %s"), *AttackLabel), nullptr, FColor::Yellow, 0.f, true);
}

void UCoreAIDebugComponent::DrawDistance(const FVector& BaseLocation) const
{
	FString DistLabel = TEXT("N/A");
    const float Distance = CoreCombat::GetDistance(CachedOwner, CachedTarget);

	if (Distance < MAX_FLT)
		DistLabel = FString::Printf(TEXT("%.0f"), Distance);

	DrawDebugString(GetWorld(), BaseLocation + FVector(0.f, 0.f, 50.f), FString::Printf(TEXT("Dist: %s"), *DistLabel), nullptr, FColor::Cyan, 0.f, true);
}
