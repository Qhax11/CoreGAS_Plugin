// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CoreEnvQueryContext_GetTarget.generated.h"

struct FEnvQueryContextData;
struct FEnvQueryInstance;

UCLASS()
class COREGAS_API UCoreEnvQueryContext_GetTarget : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
