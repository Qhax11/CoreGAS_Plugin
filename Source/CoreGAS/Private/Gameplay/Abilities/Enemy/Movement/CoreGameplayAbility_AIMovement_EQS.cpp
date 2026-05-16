// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_EQS.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_AIMoveTo.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UCoreGameplayAbility_AIMovement_EQS::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AAIController* AIController = Avatar ? Cast<AAIController>(Avatar->GetInstigatorController()) : nullptr;
	if (!AIController || !RepositionQuery)
	{
		EndReason = ECoreMovementEndReason::SetupFailed;
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	FEnvQueryRequest QueryRequest(RepositionQuery, GetAvatarActorFromActorInfo());
	QueryRequest.Execute(QueryRunMode, this, &UCoreGameplayAbility_AIMovement_EQS::OnQueryFinished);
}

void UCoreGameplayAbility_AIMovement_EQS::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid() || Result->Items.Num() == 0)
	{
		EndReason = ECoreMovementEndReason::QueryFailed;
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	const FVector BestLocation = Result->GetItemAsLocation(0);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AAIController* AIController = Avatar ? Cast<AAIController>(Avatar->GetInstigatorController()) : nullptr;
	if (!AIController)
	{
		EndReason = ECoreMovementEndReason::SetupFailed;
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	MoveTask = UCoreAbilityTask_AIMoveTo::CreateAIMoveToLocation(this, AIController, BestLocation, AcceptanceRadius);
	MoveTask->OnCompleted.AddDynamic(this, &UCoreGameplayAbility_AIMovement_EQS::OnMoveCompleted);
	MoveTask->OnAborted.AddDynamic(this,   &UCoreGameplayAbility_AIMovement_EQS::OnMoveAborted);
	MoveTask->OnFailed.AddDynamic(this,    &UCoreGameplayAbility_AIMovement_EQS::OnMoveFailed);
	MoveTask->ReadyForActivation();
}
