// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Abilities/Tasks/CoreAbilityTask_PerformTrace.h"

UCoreAbilityTask_PerformTrace* UCoreAbilityTask_PerformTrace::Instant(
	UGameplayAbility* OwningAbility,
	UCoreTraceConfig* Config,
	FVector CustomStart,
	FVector CustomEnd)
{
	UCoreAbilityTask_PerformTrace* Task = NewAbilityTask<UCoreAbilityTask_PerformTrace>(OwningAbility);
	Task->TraceConfig = Config;
	Task->CachedCustomStart = CustomStart;
	Task->CachedCustomEnd = CustomEnd;
	Task->bInstantMode = true;
	return Task;
}

UCoreAbilityTask_PerformTrace* UCoreAbilityTask_PerformTrace::Tick(
	UGameplayAbility* OwningAbility,
	UCoreTraceConfig* Config,
	float Duration,
	FVector CustomStart,
	FVector CustomEnd)
{
	UCoreAbilityTask_PerformTrace* Task = NewAbilityTask<UCoreAbilityTask_PerformTrace>(OwningAbility);
	Task->TraceConfig = Config;
	Task->CachedCustomStart = CustomStart;
	Task->CachedCustomEnd = CustomEnd;
	Task->bInstantMode = false;
	Task->Duration = Duration;
	Task->ElapsedTime = 0.f;
	Task->bTickingTask = true;
	return Task;
}

void UCoreAbilityTask_PerformTrace::Activate()
{
	if (bInstantMode)
	{
		PerformTrace();
		EndTask();
	}
}

void UCoreAbilityTask_PerformTrace::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	PerformTrace();

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= Duration)
	{
		EndTask();
	}
}

void UCoreAbilityTask_PerformTrace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UCoreAbilityTask_PerformTrace::PerformTrace()
{
	if (!TraceConfig || !Ability)
	{
		return;
	}

	AActor* AvatarActor = Ability->GetCurrentActorInfo() ? Ability->GetCurrentActorInfo()->AvatarActor.Get() : nullptr;
	if (!AvatarActor)
	{
		return;
	}

	TArray<FHitResult> HitResults = TraceConfig->Execute(AvatarActor, CachedCustomStart, CachedCustomEnd);

	if (HitResults.Num() > 0 && ShouldBroadcastAbilityTaskDelegates())
	{
		OnHit.Broadcast(HitResults);
	}
}
