// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoreAbilitySystemInterface.generated.h"

class UCoreASCBase;

UINTERFACE(BlueprintType)
class COREGAS_API UCoreAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

class COREGAS_API ICoreAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UCoreASCBase* GetCoreAbilitySystemComponent() const { return nullptr; }
};
