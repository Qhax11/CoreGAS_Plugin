// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovementBase.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_Chase.h"
#include "Gameplay/Abilities/Enemy/Movement/CoreGameplayAbility_AIMovement_EQS.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "CoreAttackData.generated.h"

class UCoreGameplayAbility_AIMovementBase;
class UEnvQuery;

USTRUCT(BlueprintType, meta = (Hidden))
struct COREGAS_API FCoreMovementConfigBase : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UCoreGameplayAbility_AIMovementBase> MovementAbilityClass;

	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreMovementConfig_Chase : public FCoreMovementConfigBase
{
	GENERATED_BODY()

	FCoreMovementConfig_Chase()
	{
		MovementAbilityClass = UCoreGameplayAbility_AIMovement_Chase::StaticClass();
	}

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Maximum time in seconds allowed for chasing the target. If exceeded, the movement ability is cancelled and the decision mechanism is triggered again. 0 means unlimited."))
	float MaxChaseTime = 0.f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Maximum distance allowed while chasing the target. If the target exceeds this distance during chase, the movement ability is cancelled and the decision mechanism is triggered again. 0 means unlimited."))
	float MaxChaseDistance = 0.f;

	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreMovementConfig_EQS : public FCoreMovementConfigBase
{
	GENERATED_BODY()

	FCoreMovementConfig_EQS()
	{
		MovementAbilityClass = UCoreGameplayAbility_AIMovement_EQS::StaticClass();
	}

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "EQS query used to find a repositioning location during combat movement."))
	TObjectPtr<UEnvQuery> RepositionQuery;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Determines how the best result is picked from the EQS query. RandomBest25Pct selects randomly from the top 25 percent of results for more varied movement."))
	TEnumAsByte<EEnvQueryRunMode::Type> QueryRunMode = EEnvQueryRunMode::RandomBest25Pct;

	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreAttackSelectionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Minimum distance required to select this attack. Target must be farther than this value."))
	float SelectionMinDistance = 0.f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Maximum distance allowed to select this attack. Target must be closer than this value."))
	float SelectionMaxDistance = 99999.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1", ClampMax = "1.0", ToolTip = "Probability weight for this attack during weighted random selection. Higher values increase selection chance."))
	float SelectionWeight = 1.0f;
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreAttackDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Identifier name for this attack entry."))
	FName AttackName;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Gameplay ability class to activate when this attack is selected."))
	TSubclassOf<UGameplayAbility> AttackAbilityClass;

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, ToolTip = "Movement behavior to execute before or during this attack."))
	TInstancedStruct<FCoreMovementConfigBase> MovementConfig;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Distance and weight settings used during attack selection."))
	FCoreAttackSelectionConfig SelectionConfig;
};

USTRUCT(BlueprintType)
struct COREGAS_API FCoreSimpleAttackData : public FCoreAttackDataBase
{
	GENERATED_BODY()

};
