// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Gameplay/AI/States/CoreStateBase.h"
#include "CoreStateManager.generated.h"

class UCoreStateBase;

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreStateManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoreStateManager();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	TArray<TSubclassOf<UCoreStateBase>> StateClassArray;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (Categories = "CoreGAS.AI.State"))
	FGameplayTag StartState;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Debug")
	bool bEnableDebug = false;

	UPROPERTY()
	FCoreStateContext StateContext;

	void Initialize(AActor* OwnerActor, UCoreASCBase* OwnerASC, AActor* TargetActor);
	void RequestStateEnter(FGameplayTag StateTag);
	void StartLogic();

private:
	void HandleStateTransitionRequested(FGameplayTag TargetStateTag);

	UPROPERTY()
	TArray<TObjectPtr<UCoreStateBase>> StateInstances;

	TObjectPtr<UCoreStateBase> CurrentState;
};
