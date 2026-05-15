// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "CoreAttackData.generated.h"

class UCoreGameplayAbility_AIMovementBase;

USTRUCT(BlueprintType)
struct COREGAS_API FCoreAttackDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	FName AttackName;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	float ScoreBias = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI", meta = (Categories = "Ability.Cooldown"))
	FGameplayTag CooldownTag;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	TSubclassOf<UCoreGameplayAbility_AIMovementBase> MovementAbilityClass;
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreSimpleAttackData : public FCoreAttackDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	TSubclassOf<UGameplayAbility> AbilityClass;
};
