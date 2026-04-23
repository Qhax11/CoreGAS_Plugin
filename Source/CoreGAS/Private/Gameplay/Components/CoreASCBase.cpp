// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Types/CoreGASTypes.h"

void UCoreASCBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreASCBase: debug: tag name: %s"), *InputTag.ToString());
		if (Spec.Ability && Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			Spec.InputPressed = true;
			if (Spec.IsActive())
			{
				AbilitySpecInputPressed(Spec);
			}
			else
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UCoreASCBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			Spec.InputPressed = false;
			if (Spec.IsActive())
			{
				AbilitySpecInputReleased(Spec);
			}
		}
	}
}

void UCoreASCBase::ListenForAttributeChange(FGameplayAttribute Attribute, const FCoreOnAttributeChanged& OnChanged)
{
	GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda([OnChanged](const FOnAttributeChangeData& Data) mutable
	{
		FCoreAttributeChangeData ChangeData;
		ChangeData.Attribute = Data.Attribute;
		ChangeData.NewValue = Data.NewValue;
		ChangeData.OldValue = Data.OldValue;
		OnChanged.Broadcast(ChangeData);
	});
}

void UCoreASCBase::ListenForTagChange(FGameplayTag Tag, const FCoreOnTagChanged& OnChanged)
{
	TSharedPtr<int32> PreviousCount = MakeShared<int32>(GetTagCount(Tag));
	RegisterGameplayTagEvent(Tag, EGameplayTagEventType::AnyCountChange).AddLambda([OnChanged, PreviousCount](const FGameplayTag InTag, int32 NewCount) mutable
	{
		const int32 OldCount = *PreviousCount;
		*PreviousCount = NewCount;

		FCoreTagChangeData ChangeData;
		ChangeData.Tag = InTag;
		ChangeData.NewCount = NewCount;
		ChangeData.OldCount = OldCount;
		if (OldCount == 0) ChangeData.ChangeEvent = ECoreTagChangeEvent::Added;
		else if (NewCount == 0) ChangeData.ChangeEvent = ECoreTagChangeEvent::Removed;
		else if (NewCount > OldCount) ChangeData.ChangeEvent = ECoreTagChangeEvent::StackAdded;
		else ChangeData.ChangeEvent = ECoreTagChangeEvent::StackRemoved;
		if (OnChanged.IsBound())
		{
			OnChanged.Execute(ChangeData);
		}
	});
}

void UCoreASCBase::NotifyGASDataInitialized()
{
	OnGASDataInitialized.Broadcast();
}
