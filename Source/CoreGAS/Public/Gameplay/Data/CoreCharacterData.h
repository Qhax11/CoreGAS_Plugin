// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Gameplay/Abilities/CoreGameplayAbilityBase.h"
#include "CoreCharacterData.generated.h"

class UDataTable;
class UInputAction;

USTRUCT(BlueprintType)
struct FCoreAbilityInputBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

UCLASS(BlueprintType)
class COREGAS_API UCoreCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Abilities")
	TArray<TSubclassOf<UCoreGameplayAbilityBase>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Effects")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Attributes")
	TSoftObjectPtr<UDataTable> AttributeInitializationData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Tags")
	FGameplayTagContainer PermanentTags;
};
