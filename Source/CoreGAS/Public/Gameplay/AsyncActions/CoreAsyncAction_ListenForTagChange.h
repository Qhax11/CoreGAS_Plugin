// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "CoreAsyncAction_ListenForTagChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoreTagChangeAsyncDelegate, const FCoreTagChangeData&, ChangeData);

UCLASS()
class COREGAS_API UAsyncAction_ListenForTagChange : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoreTagChangeAsyncDelegate OnChanged;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Async",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncAction_ListenForTagChange* ListenForTagChange(UObject* WorldContextObject, UAbilitySystemComponent* ASC, FGameplayTag Tag);

	virtual void Activate() override;
	virtual void Cancel() override;

private:
	TWeakObjectPtr<UAbilitySystemComponent> TrackedASC;
	FGameplayTag TrackedTag;
	FDelegateHandle DelegateHandle;
	int32 PreviousCount = 0;
};
