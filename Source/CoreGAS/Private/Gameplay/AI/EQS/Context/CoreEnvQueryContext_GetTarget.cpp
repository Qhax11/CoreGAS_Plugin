// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/EQS/Context/CoreEnvQueryContext_GetTarget.h"
#include "Gameplay/Interfaces/ICoreCombatInterface.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UCoreEnvQueryContext_GetTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    APawn* Pawn = Cast<APawn>(QueryInstance.Owner.Get());
    if (!Pawn)
        return;

    ICoreCombatInterface* CombatInterface = Cast<ICoreCombatInterface>(Pawn);
    if (!CombatInterface)
        return;

    AActor* TargetActor = CombatInterface->GetCurrentTarget();
    if (!TargetActor)
        return;

    UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
}
