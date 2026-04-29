// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Engine/HitResult.h"
#include "Gameplay/Tracing/CoreTraceConfig.h"
#include "CoreAbilityTask_PerformTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTraceCoreHit, const TArray<FHitResult>&, HitResults);

UCLASS()
class COREGAS_API UCoreAbilityTask_PerformTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnTraceCoreHit OnHit;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCoreAbilityTask_PerformTrace* PerformTraceInstant(
		UGameplayAbility* OwningAbility,
		UCoreTraceConfig* Config,
		AActor* WeaponActor,
		FVector CustomStart = FVector::ZeroVector,
		FVector CustomEnd = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCoreAbilityTask_PerformTrace* PerformTraceTick(
		UGameplayAbility* OwningAbility,
		UCoreTraceConfig* Config,
		float Duration,
		AActor* WeaponActor,
		FVector CustomStart = FVector::ZeroVector,
		FVector CustomEnd = FVector::ZeroVector);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	UPROPERTY()
	TObjectPtr<UCoreTraceConfig> TraceConfig;

	UPROPERTY()
	TObjectPtr<AActor> CachedWeaponActor;

	FVector CachedCustomStart;
	FVector CachedCustomEnd;
	FVector PreviousStartLocation;
	FVector PreviousEndLocation;
	bool bHasPreviousLocations = false;
	bool bInstantMode = true;
	float Duration = 0.f;
	float ElapsedTime = 0.f;

	void PerformTrace();
};
