// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Data/CoreCharacterData.h"
#include "CoreHeroData.generated.h"

UCLASS(BlueprintType)
class COREGAS_API UCoreHeroData : public UCoreCharacterData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Input")
	TArray<FCoreAbilityInputBinding> AbilityInputBindings;
};
