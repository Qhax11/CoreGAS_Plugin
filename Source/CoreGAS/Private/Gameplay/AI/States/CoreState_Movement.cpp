// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Movement.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "Gameplay/Debug/CoreGameplayLog.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Tags/CoreAITags.h"
#include "Abilities/GameplayAbilityTypes.h"

UCoreState_Movement::UCoreState_Movement()
{
	StateTag = CoreGAS::AI::TAG_State_Combat_Movement;
}

void UCoreState_Movement::OnEnter(UCoreStateManager* StateManager)
{
    Super::OnEnter(StateManager);

    // --- Validation ---
    if (!Context.TargetActor || !Context.BehaviorDecision || !Context.OwnerASC)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - TargetActor:%s BehaviorDecision:%s OwnerASC:%s",
            Context.TargetActor ? TEXT("OK") : TEXT("NULL"),
            Context.BehaviorDecision ? TEXT("OK") : TEXT("NULL"),
            Context.OwnerASC ? TEXT("OK") : TEXT("NULL"));
        return;
    }

    UCoreASCBase* ASC = Context.OwnerASC;

    // --- Config ---
    const FCoreAttackDataBase* BestAttack = Context.BehaviorDecision->GetBestAttack();
    if (!BestAttack)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - BestAttack is NULL");
        return;
    }

    const FCoreMovementConfigBase* MovementConfig = BestAttack->MovementConfig.GetPtr<FCoreMovementConfigBase>();
    if (!MovementConfig || !MovementConfig->MovementAbilityClass)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - No MovementAbilityClass on BestAttack");
        return;
    }

    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(MovementConfig->MovementAbilityClass);
    if (!Spec)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - AbilitySpec not found");
        return;
    }
    ActiveAbilityHandle = Spec->Handle;

    // --- Listen ---
    // Bind before activating to avoid missing AlreadyAtGoal completions
    EndListenerHandle = ASC->ListenForAbilityEndedByHandle(ActiveAbilityHandle,
        [this](const FAbilityEndedData& EndData) { OnMovementAbilityEnded(EndData); });

    // --- Activate ---
    FGameplayEventData EventData = BuildMovementEventData(BestAttack);
    if (!ASC->ActivateAbilityByClassAndReturnHandle(MovementConfig->MovementAbilityClass, ActiveAbilityHandle, EventData))
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "ActivateAbility FAILED - ability did not activate");
        ASC->StopListeningForAbilityEnded(EndListenerHandle);
        EndListenerHandle = FDelegateHandle();
        return;
    }

    CORE_AI_LOG(LogCoreAIMovement, Log, "Ability activated. Listening for end...");
}

FGameplayEventData UCoreState_Movement::BuildMovementEventData(const FCoreAttackDataBase* BestAttack) const
{
    FGameplayEventData EventData;
    EventData.Target = Context.TargetActor;

    const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(BestAttack);
    if (SimpleAttack && SimpleAttack->AbilityClass)
    {
        UCoreGameplayAbility_AttackBase* CDO = SimpleAttack->AbilityClass->GetDefaultObject<UCoreGameplayAbility_AttackBase>();
        EventData.EventMagnitude = CDO ? CDO->MaxRange : 150.f;
        CORE_AI_LOG(LogCoreAIMovement, Log, "AcceptanceRadius set to: %.1f", EventData.EventMagnitude);
    }

    FCoreMovementConfigBase* ConfigPtr = const_cast<FCoreMovementConfigBase*>(BestAttack->MovementConfig.GetPtr<FCoreMovementConfigBase>());
    if (ConfigPtr)
    {
        EventData.TargetData.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(ConfigPtr, [](FGameplayAbilityTargetData*) {}));
    }

    return EventData;
}

void UCoreState_Movement::OnMovementAbilityEnded(const FAbilityEndedData& EndData)
{
    UCoreGameplayAbility_AIMovementBase* MovementAbility = Cast<UCoreGameplayAbility_AIMovementBase>(EndData.AbilityThatEnded);
    ECoreMovementEndReason Reason = MovementAbility ? MovementAbility->EndReason : ECoreMovementEndReason::Cancelled;
    CORE_AI_LOG(LogCoreAIMovement, Log, "Ability ended - Reason:%d bWasCancelled:%s", (int32)Reason, EndData.bWasCancelled ? TEXT("true") : TEXT("false"));

    switch (Reason)
    {
    case ECoreMovementEndReason::Success:
        if (TransitionTag.IsValid())
            RequestTransition(TransitionTag);
        break;
    case ECoreMovementEndReason::SetupFailed:
        CORE_AI_LOG(LogCoreAIMovement, Warning, "Movement SetupFailed - no transition");
        break;
    default:
        RequestTransition(Context.BehaviorDecision->DecideNextState());
        break;
    }
}

void UCoreState_Movement::OnExit(UCoreStateManager* StateManager)
{
    UCoreASCBase* ASC = Context.OwnerASC;
    if (ASC && EndListenerHandle.IsValid())
    {
        ASC->StopListeningForAbilityEnded(EndListenerHandle);
    }

    EndListenerHandle = FDelegateHandle();
    ActiveAbilityHandle = FGameplayAbilitySpecHandle();

    Super::OnExit(StateManager);
}