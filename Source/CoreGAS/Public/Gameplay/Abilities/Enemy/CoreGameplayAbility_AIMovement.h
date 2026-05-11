// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "CoreGameplayAbility_AIMovement.generated.h"

class UCoreAbilityTask_AIMoveTo;

UCLASS()
class COREGAS_API UCoreGameplayAbility_AIMovement : public UCoreGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_AIMovement();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	float AcceptanceRadius = 50.f;

private:
	UPROPERTY()
	TObjectPtr<UCoreAbilityTask_AIMoveTo> MoveTask;

	UFUNCTION()
	void OnMoveCompleted();

	UFUNCTION()
	void OnMoveAborted();

	UFUNCTION()
	void OnMoveFailed();
};
