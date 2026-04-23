// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Gameplay/Tags/CoreInputTags.h"
#include "CoreGameplayAbilityBase.generated.h"


UCLASS()
class COREGAS_API UCoreGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Ability")
	FGameplayTag AbilityInputTag;
};
