// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AsyncActions/CoreAsyncAction_ListenForAttributeChange.h"
#include "Gameplay/Components/CoreASCBase.h"

UAsyncAction_ListenForAttributeChange* UAsyncAction_ListenForAttributeChange::ListenForAttributeChange(UCoreASCBase* ASC, FGameplayAttribute Attribute)
{
	UAsyncAction_ListenForAttributeChange* Action = NewObject<UAsyncAction_ListenForAttributeChange>();
	Action->CachedASC = ASC;
	Action->CachedAttribute = Attribute;
	return Action;
}

void UAsyncAction_ListenForAttributeChange::Activate()
{
	if (!CachedASC.IsValid())
	{
		return;
	}

	CachedASC->GetGameplayAttributeValueChangeDelegate(CachedAttribute).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		FCoreAttributeChangeData ChangeData;
		ChangeData.Attribute = Data.Attribute;
		ChangeData.NewValue = Data.NewValue;
		ChangeData.OldValue = Data.OldValue;
		OnChanged.Broadcast(ChangeData);
	});
}
