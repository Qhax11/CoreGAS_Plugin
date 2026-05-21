// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreAIDebugComponent.generated.h"

class UCoreStateManager;
class UCoreAIBehaviorDecision;

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreAIDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoreAIDebugComponent();

	void Initialize(AActor* OwnerActor, AActor* TargetActor, UCoreStateManager* InStateManager, UCoreAIBehaviorDecision* InBehaviorDecision);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Debug")
	bool bEnableDebug = false;

private:
	void DrawState(const FVector& BaseLocation) const;
	void DrawAttack(const FVector& BaseLocation) const;
	void DrawDistance(const FVector& BaseLocation) const;

	UPROPERTY()
	TObjectPtr<UCoreStateManager> StateManager;

	UPROPERTY()
	TObjectPtr<UCoreAIBehaviorDecision> BehaviorDecision;

	UPROPERTY()
	TObjectPtr<AActor> CachedOwner;

	UPROPERTY()
	TObjectPtr<AActor> CachedTarget;
};
