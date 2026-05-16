// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "CoreAbilityTask_AIMoveTo.generated.h"

class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCoreAIMoveDelegate);

UCLASS()
class COREGAS_API UCoreAbilityTask_AIMoveTo : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoreAIMoveDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FCoreAIMoveDelegate OnAborted;

	UPROPERTY(BlueprintAssignable)
	FCoreAIMoveDelegate OnFailed;

	UPROPERTY(BlueprintAssignable)
	FCoreAIMoveDelegate OnCancelled;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|AI|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCoreAbilityTask_AIMoveTo* CreateAIMoveToActor(
		UGameplayAbility* OwningAbility,
		AAIController* Controller,
		AActor* TargetActor,
		float AcceptanceRadius,
		float MaxChaseTime = 0.f,
		float MaxChaseDistance = 0.f);

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|AI|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCoreAbilityTask_AIMoveTo* CreateAIMoveToLocation(
		UGameplayAbility* OwningAbility,
		AAIController* Controller,
		FVector Location,
		float AcceptanceRadius);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	UPROPERTY()
	TObjectPtr<AAIController> CachedController;

	UPROPERTY()
	TObjectPtr<AActor> CachedTarget;

	UPROPERTY()
	FVector CachedLocation;

	UPROPERTY()
	TObjectPtr<AActor> CachedOwnerActor;

	float CachedAcceptanceRadius = 50.f;
	float MaxChaseTime = 0.f;
	float MaxChaseDistance = 0.f;
	float ElapsedChaseTime = 0.f;
	bool bMoveToLocation = false;

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
