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
};
