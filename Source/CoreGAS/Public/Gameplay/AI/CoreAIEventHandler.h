// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreAIEventHandler.generated.h"

class UCoreStateManager;
class UAbilitySystemComponent;

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreAIEventHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	void Initialize(UCoreStateManager* InStateManager, UAbilitySystemComponent* InASC);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnHealthEmptyTagChanged(const FGameplayTag Tag, int32 NewCount);

	UPROPERTY()
	TObjectPtr<UCoreStateManager> CachedStateManager;

	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
};
