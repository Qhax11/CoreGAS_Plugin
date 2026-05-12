// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreDecisionOptionData.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class COREGAS_API UCoreDecisionOptionData : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsEnable() const { return true; }
	virtual bool PassesChance() const { return true; }
	virtual float GetScore() const { return 0.f; }
};
