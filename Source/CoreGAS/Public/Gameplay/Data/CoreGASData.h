// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "CoreGASData.generated.h"

class UDataTable;
class UInputAction;

USTRUCT(BlueprintType)
struct FCoreAbilityInputBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "CoreGAS.Input"))
	FGameplayTag InputTag;
};

UCLASS(BlueprintType)
class COREGAS_API UCoreGASData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities|Combat|Attack")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> AttackAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities|Combat|Combo")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> ComboAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities|Combat|Reactions")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> ReactionAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities|Movement")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> MovementAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities|Passive")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> PassiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities|Utility")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> UtilityAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Effects")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Attributes")
	TSoftObjectPtr<UDataTable> AttributeInitializationData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Tags")
	FGameplayTagContainer PermanentTags;
};
