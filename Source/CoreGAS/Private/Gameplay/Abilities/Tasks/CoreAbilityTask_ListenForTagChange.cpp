// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Tasks/CoreAbilityTask_ListenForTagChange.h"
#include "AbilitySystemComponent.h"

UCoreAbilityTask_ListenForTagChange* UCoreAbilityTask_ListenForTagChange::ListenForTagChange(
	UGameplayAbility* OwningAbility, FGameplayTag Tag)
{
	UCoreAbilityTask_ListenForTagChange* Task = NewAbilityTask<UCoreAbilityTask_ListenForTagChange>(OwningAbility);
	Task->TrackedTag = Tag;
	return Task;
}

void UCoreAbilityTask_ListenForTagChange::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC || !TrackedTag.IsValid())
	{
		EndTask();
		return;
	}

	PreviousCount = ASC->GetTagCount(TrackedTag);
	DelegateHandle = ASC->RegisterGameplayTagEvent(TrackedTag, EGameplayTagEventType::AnyCountChange).AddLambda(
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				const int32 OldCount = PreviousCount;
				PreviousCount = NewCount;

				FCoreTagChangeData ChangeData;
				ChangeData.Tag = Tag;
				ChangeData.NewCount = NewCount;
				ChangeData.OldCount = OldCount;
				if (OldCount == 0) ChangeData.ChangeEvent = ECoreTagChangeEvent::Added;
				else if (NewCount == 0) ChangeData.ChangeEvent = ECoreTagChangeEvent::Removed;
				else if (NewCount > OldCount) ChangeData.ChangeEvent = ECoreTagChangeEvent::StackAdded;
				else ChangeData.ChangeEvent = ECoreTagChangeEvent::StackRemoved;
				OnChanged.Broadcast(ChangeData);
			}
		});
}

void UCoreAbilityTask_ListenForTagChange::OnDestroy(bool bInOwnerFinished)
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->RegisterGameplayTagEvent(TrackedTag, EGameplayTagEventType::AnyCountChange).Remove(DelegateHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}
