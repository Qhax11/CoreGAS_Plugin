// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "CoreASCBase.generated.h"

// Forward declare to break circular include with CoreGASTypes.h
class FCoreOnAttributeChanged;
struct FCoreAttributeChangeData;
class FCoreOnTagChanged;
struct FCoreTagChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGASDataInitialized);

UCLASS()
class COREGAS_API UCoreASCBase : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	void NotifyGASDataInitialized();

	void ListenForAttributeChange(FGameplayAttribute Attribute, const FCoreOnAttributeChanged& OnChanged);
	void ListenForTagChange(FGameplayTag Tag, const FCoreOnTagChanged& OnChanged);

	UPROPERTY(BlueprintAssignable, Category = "CoreGAS")
	FOnGASDataInitialized OnGASDataInitialized;

	FDelegateHandle ListenForAbilityEndedByHandle(FGameplayAbilitySpecHandle SpecHandle, TFunction<void(const FAbilityEndedData&)> Callback);
	bool ActivateAbilityByClassAndReturnHandle(TSubclassOf<UGameplayAbility> AbilityClass, FGameplayAbilitySpecHandle& OutHandle);
	void StopListeningForAbilityEnded(FDelegateHandle Handle);

private:
	struct FCoreAbilityEndListener
	{
		FGameplayAbilitySpecHandle WatchedHandle;
		TFunction<void(const FAbilityEndedData&)> Callback;
		FDelegateHandle DelegateHandle;
	};

	TArray<FCoreAbilityEndListener> ActiveEndListeners;
	FDelegateHandle MasterAbilityEndedHandle;

	void OnAnyAbilityEnded(const FAbilityEndedData& Data);
};
