// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "CoreGameplayAbility_AIMovementBase.generated.h"

class UCoreAbilityTask_AIMoveTo;

UENUM(BlueprintType)
enum class ECoreMovementEndReason : uint8
{
	Success,
	Cancelled,
	SetupFailed,
	QueryFailed,
	MoveFailed
};

UCLASS(Abstract)
class COREGAS_API UCoreGameplayAbility_AIMovementBase : public UCoreGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_AIMovementBase();

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	ECoreMovementEndReason EndReason = ECoreMovementEndReason::Cancelled;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	float AcceptanceRadius = 50.f;

	UPROPERTY()
	TObjectPtr<UCoreAbilityTask_AIMoveTo> MoveTask;

	UFUNCTION()
	void OnMoveCompleted();

	UFUNCTION()
	void OnMoveAborted();

	UFUNCTION()
	void OnMoveFailed();
};
