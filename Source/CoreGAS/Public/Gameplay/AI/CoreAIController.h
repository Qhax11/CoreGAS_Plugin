// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CoreAIController.generated.h"

class UCoreStateManager;

UCLASS()
class COREGAS_API ACoreAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACoreAIController();

	UCoreStateManager* GetStateManager() const { return StateManager; }

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreStateManager> StateManager;
};
