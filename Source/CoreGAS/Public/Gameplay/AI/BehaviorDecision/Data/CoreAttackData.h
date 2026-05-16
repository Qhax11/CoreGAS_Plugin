// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_Chase.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_EQS.h"
#include "StructUtils/InstancedStruct.h"
#include "CoreAttackData.generated.h"

class UCoreGameplayAbility_AIMovementBase;

USTRUCT(BlueprintType, meta = (Hidden))
struct COREGAS_API FCoreMovementConfigBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	TSubclassOf<UCoreGameplayAbility_AIMovementBase> MovementAbilityClass;
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreMovementConfig_Chase : public FCoreMovementConfigBase
{
	GENERATED_BODY()

	FCoreMovementConfig_Chase()
	{
		MovementAbilityClass = UCoreGameplayAbility_AIMovement_Chase::StaticClass();
	}

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	float MaxChaseTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement")
	float MaxChaseDistance = 0.f;
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreMovementConfig_EQS : public FCoreMovementConfigBase
{
	GENERATED_BODY()

	FCoreMovementConfig_EQS()
	{
		MovementAbilityClass = UCoreGameplayAbility_AIMovement_EQS::StaticClass();
	}
};

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

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI|Movement", meta = (ExcludeBaseStruct))
	TInstancedStruct<FCoreMovementConfigBase> MovementConfig;
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreSimpleAttackData : public FCoreAttackDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AI")
	TSubclassOf<UGameplayAbility> AbilityClass;
};
