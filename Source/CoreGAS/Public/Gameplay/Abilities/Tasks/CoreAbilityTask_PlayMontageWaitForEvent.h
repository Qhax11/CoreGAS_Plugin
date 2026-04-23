// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "CoreAbilityTask_PlayMontageWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCoreAbilityTaskMontageDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class COREGAS_API UCoreAbilityTask_PlayMontageWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoreAbilityTaskMontageDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FCoreAbilityTaskMontageDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FCoreAbilityTaskMontageDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FCoreAbilityTaskMontageDelegate OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FCoreAbilityTaskMontageDelegate EventReceived;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCoreAbilityTask_PlayMontageWaitForEvent* PlayMontageAndWaitForEvent(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		FGameplayTagContainer EventTags,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		float AnimRootMotionTranslationScale = 1.f);

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void StopPlayingMontage();
	void UnbindAllDelegates();

private:
	UPROPERTY()
	UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY()
	FGameplayTagContainer EventTags;

	float Rate = 1.f;
	FName StartSection;
	float AnimRootMotionTranslationScale = 1.f;

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
};
