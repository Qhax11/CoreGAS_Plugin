// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/States/CoreStateBase.h"
#include "CoreState_Dead.generated.h"

UCLASS()
class COREGAS_API UCoreState_Dead : public UCoreStateBase
{
	GENERATED_BODY()

public:
	UCoreState_Dead();

	virtual void OnEnter(UCoreStateManager* StateManager) override;
};
