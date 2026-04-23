// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AsyncActions/CoreAsyncAction_ListenForTagChange.h"

UAsyncAction_ListenForTagChange* UAsyncAction_ListenForTagChange::ListenForTagChange(
	UObject* WorldContextObject, UAbilitySystemComponent* ASC, FGameplayTag Tag)
{
	UAsyncAction_ListenForTagChange* Action = NewObject<UAsyncAction_ListenForTagChange>();
	Action->TrackedASC = ASC;
	Action->TrackedTag = Tag;
	return Action;
}

void UAsyncAction_ListenForTagChange::Activate()
{
	UAbilitySystemComponent* ASC = TrackedASC.Get();
	if (!ASC || !TrackedTag.IsValid())
	{
		Cancel();
		return;
	}

	PreviousCount = ASC->GetTagCount(TrackedTag);
	DelegateHandle = ASC->RegisterGameplayTagEvent(TrackedTag, EGameplayTagEventType::AnyCountChange).AddLambda(
		[this](const FGameplayTag Tag, int32 NewCount)
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
		});
}

void UAsyncAction_ListenForTagChange::Cancel()
{
	if (UAbilitySystemComponent* ASC = TrackedASC.Get())
	{
		ASC->RegisterGameplayTagEvent(TrackedTag, EGameplayTagEventType::AnyCountChange).Remove(DelegateHandle);
	}

	Super::Cancel();
}
