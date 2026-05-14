// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Data/CoreGASData.h"
#include "CoreHeroGASData.generated.h"

UCLASS(BlueprintType)
class COREGAS_API UCoreHeroGASData : public UCoreGASData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Input")
	TArray<FCoreAbilityInputBinding> AbilityInputBindings;
};
