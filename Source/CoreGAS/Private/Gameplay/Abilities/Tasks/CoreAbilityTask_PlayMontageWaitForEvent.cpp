// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Tasks/CoreAbilityTask_PlayMontageWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Async/TaskGraphInterfaces.h"

UCoreAbilityTask_PlayMontageWaitForEvent* UCoreAbilityTask_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	UAnimMontage* MontageToPlay,
	FGameplayTagContainer EventTags,
	float Rate,
	FName StartSection,
	float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UCoreAbilityTask_PlayMontageWaitForEvent* Task = NewAbilityTask<UCoreAbilityTask_PlayMontageWaitForEvent>(OwningAbility, TaskInstanceName);
	Task->MontageToPlay = MontageToPlay;
	Task->EventTags = EventTags;
	Task->Rate = Rate;
	Task->StartSection = StartSection;
	Task->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	return Task;
}

void UCoreAbilityTask_PlayMontageWaitForEvent::Activate()
{
	if (!Ability)
	{
		return;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	UAnimInstance* AnimInstance = ActorInfo ? ActorInfo->GetAnimInstance() : nullptr;

	if (!AnimInstance || !MontageToPlay)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
		EndTask();
		return;
	}

	if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
	{
		if (!ShouldBroadcastAbilityTaskDelegates())
		{
			return;
		}

		CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UCoreAbilityTask_PlayMontageWaitForEvent::OnAbilityCancelled);

		BlendingOutDelegate.BindUObject(this, &UCoreAbilityTask_PlayMontageWaitForEvent::OnMontageBlendingOut);
		AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

		MontageEndedDelegate.BindUObject(this, &UCoreAbilityTask_PlayMontageWaitForEvent::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

		ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
		if (Character)
		{
			Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
		}

		if (AbilitySystemComponent.IsValid())
		{
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(
				EventTags,
				FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UCoreAbilityTask_PlayMontageWaitForEvent::OnGameplayEvent));
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
		EndTask();
	}
}

void UCoreAbilityTask_PlayMontageWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent.IsValid());
	OnAbilityCancelled();
	Super::ExternalCancel();
}

FString UCoreAbilityTask_PlayMontageWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo ? ActorInfo->GetAnimInstance() : nullptr;
		if (AnimInstance)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}
	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"),
		*GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UCoreAbilityTask_PlayMontageWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);

		if (AbilityEnded && MontageToPlay)
		{
			StopPlayingMontage();
		}
	}

	UnbindAllDelegates();
	Super::OnDestroy(AbilityEnded);
}

void UCoreAbilityTask_PlayMontageWaitForEvent::StopPlayingMontage()
{
	if (!Ability)
	{
		return;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AbilitySystemComponent.IsValid() && AbilitySystemComponent->GetAnimatingAbility() == Ability
		&& Ability->GetCurrentMontage() == MontageToPlay)
	{
		FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
		if (MontageInstance)
		{
			MontageInstance->OnMontageBlendingOutStarted.Unbind();
			MontageInstance->OnMontageEnded.Unbind();
		}

		AbilitySystemComponent->CurrentMontageStop();
	}
}

void UCoreAbilityTask_PlayMontageWaitForEvent::UnbindAllDelegates()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}
}

void UCoreAbilityTask_PlayMontageWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay && Montage == MontageToPlay)
	{
		AbilitySystemComponent->ClearAnimatingAbility(Ability);

		ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
		if (Character)
		{
			Character->SetAnimRootMotionTranslationScale(1.f);
		}
	}

	TWeakObjectPtr<UCoreAbilityTask_PlayMontageWaitForEvent> WeakThis(this);
	AsyncTask(ENamedThreads::GameThread, [WeakThis, bInterrupted]()
	{
		if (!WeakThis.IsValid() || !WeakThis->ShouldBroadcastAbilityTaskDelegates())
		{
			return;
		}

		if (bInterrupted)
		{
			WeakThis->OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
		else
		{
			WeakThis->OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	});
}

void UCoreAbilityTask_PlayMontageWaitForEvent::OnAbilityCancelled()
{
	StopPlayingMontage();

	TWeakObjectPtr<UCoreAbilityTask_PlayMontageWaitForEvent> WeakThis(this);
	AsyncTask(ENamedThreads::GameThread, [WeakThis]()
	{
		if (!WeakThis.IsValid())
		{
			return;
		}

		if (WeakThis->ShouldBroadcastAbilityTaskDelegates())
		{
			WeakThis->OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
		WeakThis->EndTask();
	});
}

void UCoreAbilityTask_PlayMontageWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		TWeakObjectPtr<UCoreAbilityTask_PlayMontageWaitForEvent> WeakThis(this);
		AsyncTask(ENamedThreads::GameThread, [WeakThis]()
		{
			if (!WeakThis.IsValid())
			{
				return;
			}

			if (WeakThis->ShouldBroadcastAbilityTaskDelegates())
			{
				WeakThis->OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
			}
			WeakThis->EndTask();
		});
	}
	else
	{
		EndTask();
	}
}

void UCoreAbilityTask_PlayMontageWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	FGameplayEventData PayloadCopy = Payload ? *Payload : FGameplayEventData();

	TWeakObjectPtr<UCoreAbilityTask_PlayMontageWaitForEvent> WeakThis(this);
	AsyncTask(ENamedThreads::GameThread, [WeakThis, EventTag, PayloadCopy]()
	{
		if (!WeakThis.IsValid() || !WeakThis->ShouldBroadcastAbilityTaskDelegates())
		{
			return;
		}
		WeakThis->EventReceived.Broadcast(EventTag, PayloadCopy);
	});
}
