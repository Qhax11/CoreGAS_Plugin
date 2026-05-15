// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICoreCombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCoreCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class COREGAS_API ICoreCombatInterface
{
	GENERATED_BODY()

public:
	virtual AActor* GetCurrentTarget() const { return nullptr; }
};
