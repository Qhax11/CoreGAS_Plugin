// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Tasks/CoreAbilityTask_ListenForAttributeChange.h"
#include "AbilitySystemComponent.h"

UCoreAbilityTask_ListenForAttributeChange* UCoreAbilityTask_ListenForAttributeChange::ListenForAttributeChange(
	UGameplayAbility* OwningAbility, FGameplayAttribute Attribute)
{
	UCoreAbilityTask_ListenForAttributeChange* Task = NewAbilityTask<UCoreAbilityTask_ListenForAttributeChange>(OwningAbility);
	Task->TrackedAttribute = Attribute;
	return Task;
}

void UCoreAbilityTask_ListenForAttributeChange::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC || !TrackedAttribute.IsValid())
	{
		EndTask();
		return;
	}

	DelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(TrackedAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				FCoreAttributeChangeData ChangeData;
				ChangeData.Attribute = Data.Attribute;
				ChangeData.NewValue = Data.NewValue;
				ChangeData.OldValue = Data.OldValue;
				OnChanged.Broadcast(ChangeData);
			}
		});
}

void UCoreAbilityTask_ListenForAttributeChange::OnDestroy(bool bInOwnerFinished)
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(TrackedAttribute).Remove(DelegateHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}
