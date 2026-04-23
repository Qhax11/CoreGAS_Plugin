// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "Gameplay/Abilities/Tasks/CoreAbilityTask_PlayMontageWaitForEvent.h"
#include "CoreGameplayAbility_Montage.generated.h"

class UAbilityTask_WaitDelay;

UENUM(BlueprintType)
enum class EMontageEndPolicy : uint8
{
	Standard   UMETA(DisplayName = "Standard"),
	Never      UMETA(DisplayName = "Never"),
};

UCLASS()
class COREGAS_API UCoreGameplayAbility_Montage : public UCoreGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Montage")
	UAnimMontage* AbilityMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Montage")
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Montage")
	FName StartSection = NAME_None;

	// Standard: ability ends when the montage completes or is interrupted. Never: ability must be ended manually.
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Montage")
	EMontageEndPolicy MontageEndPolicy = EMontageEndPolicy::Standard;

	// Extra seconds to keep the ability alive after blend-out begins. Ignored on interruption/cancellation. Only used when MontageEndPolicy is Standard.
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Montage")
	float PostBlendOutDelay = 0.f;

	// Gameplay events with these tags received during the montage will trigger OnEventReceived.
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Montage")
	FGameplayTagContainer WaitForEventTags;

	UFUNCTION()
	void OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) {}

	void HandleMontageEvent(bool bWasCancelled);
	void CancelBlendOutDelay();

private:
	UPROPERTY()
	UCoreAbilityTask_PlayMontageWaitForEvent* MontageTask = nullptr;

	UPROPERTY()
	UAbilityTask_WaitDelay* BlendOutDelayTask = nullptr;

	UFUNCTION()
	void OnBlendOutDelayFinished();
};
