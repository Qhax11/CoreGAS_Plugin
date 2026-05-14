// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Gameplay/Subsystems/CoreSpawnSubsystem.h"
#include "CoreAIController.generated.h"

class UCoreStateManager;
class UCoreAIEventHandler;
class UCoreAIBehaviorDecision;
class UCoreEnemyArchetypeData;

UCLASS()
class COREGAS_API ACoreAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACoreAIController();

	UCoreStateManager* GetStateManager() const { return StateManager; }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreStateManager> StateManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreAIEventHandler> EventHandler;

	UPROPERTY()
	TObjectPtr<UCoreAIBehaviorDecision> BehaviorDecision;

private:
	UFUNCTION()
	void OnHeroSpawned(const FHeroSpawnData& HeroSpawnData);
};
