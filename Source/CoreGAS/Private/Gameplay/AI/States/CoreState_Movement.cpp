// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/States/CoreState_Movement.h"
#include "Gameplay/AI/CoreStateManager.h"
#include "Gameplay/AI/BehaviorDecision/CoreAIBehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreAttackData.h"
#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
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

    if (!Context.TargetActor || !Context.BehaviorDecision)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - TargetActor:%s BehaviorDecision:%s",
            Context.TargetActor ? TEXT("OK") : TEXT("NULL"),
            Context.BehaviorDecision ? TEXT("OK") : TEXT("NULL"));
        return;
    }

    UCoreASCBase* ASC = Context.OwnerASC;
    if (!ASC)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - OwnerASC is NULL");
        return;
    }

    const FCoreAttackDataBase* BestAttack = Context.BehaviorDecision->GetBestAttack();
    if (!BestAttack || !BestAttack->MovementAbilityClass)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - No MovementAbilityClass on BestAttack");
        return;
    }

    // Fetch the handle before activation so the listener can be bound first
    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(BestAttack->MovementAbilityClass);
    if (!Spec)
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "OnEnter ABORTED - AbilitySpec not found");
        return;
    }
    ActiveAbilityHandle = Spec->Handle;

    // Bind the listener before activating to avoid missing AlreadyAtGoal completions
    EndListenerHandle = ASC->ListenForAbilityEndedByHandle(ActiveAbilityHandle, [this](const FAbilityEndedData& EndData)
        {
            CORE_AI_LOG(LogCoreAIMovement, Log, "Ability ended - bWasCancelled:%s",
                EndData.bWasCancelled ? TEXT("true") : TEXT("false"));

            if (TransitionTag.IsValid())
            {
                RequestTransition(TransitionTag);
            }
            else
            {
                CORE_AI_LOG(LogCoreAIMovement, Warning, "Ability ended but TransitionTag is INVALID");
            }
        });

    FGameplayEventData EventData;
    EventData.Target = Context.TargetActor;

    const FCoreSimpleAttackData* SimpleAttack = static_cast<const FCoreSimpleAttackData*>(BestAttack);
    if (SimpleAttack && SimpleAttack->AbilityClass)
    {
        UCoreGameplayAbility_AttackBase* CDO = SimpleAttack->AbilityClass->GetDefaultObject<UCoreGameplayAbility_AttackBase>();
        EventData.EventMagnitude = CDO ? CDO->MaxRange : 150.f;
        CORE_AI_LOG(LogCoreAIMovement, Log, "AcceptanceRadius set to: %.1f", EventData.EventMagnitude);
    }

    // Activate after listener is bound - handles AlreadyAtGoal completing synchronously
    if (!ASC->ActivateAbilityByClassAndReturnHandle(BestAttack->MovementAbilityClass, ActiveAbilityHandle, EventData))
    {
        CORE_AI_LOG(LogCoreAIMovement, Warning, "ActivateAbility FAILED - ability did not activate");
        ASC->StopListeningForAbilityEnded(EndListenerHandle);
        EndListenerHandle = FDelegateHandle();
        return;
    }

    CORE_AI_LOG(LogCoreAIMovement, Log, "Ability activated. Listening for end...");
}

void UCoreState_Movement::OnExit(UCoreStateManager* StateManager)
{
	UCoreASCBase* ASC = Context.OwnerASC;
	if (ASC && EndListenerHandle.IsValid())
	{
		ASC->StopListeningForAbilityEnded(EndListenerHandle);
	}

	EndListenerHandle   = FDelegateHandle();
	ActiveAbilityHandle = FGameplayAbilitySpecHandle();

	Super::OnExit(StateManager);
}
