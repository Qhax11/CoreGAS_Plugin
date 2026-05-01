// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CoreGASLibrary.generated.h"

class UCoreASCBase;
class UAttributeSet;
class UCoreCharacterData;
class UAbilitySystemComponent;

UCLASS()
class COREGAS_API UCoreGASLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ApplyCharacterData(UCoreASCBase* ASC, UAttributeSet* AttributeSet, UCoreCharacterData* Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CoreGAS")
	static UCoreASCBase* GetCoreASCFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CoreGAS|Cooldown")
	static bool GetCooldownRemainingAndDurationByContainer(
		UAbilitySystemComponent* ASC,
		FGameplayTagContainer CooldownTags,
		float& OutTimeRemaining,
		float& OutDuration);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CoreGAS|Cooldown")
	static bool GetCooldownRemainingAndDurationByTag(
		UAbilitySystemComponent* ASC,
		FGameplayTag CooldownTag,
		float& OutTimeRemaining,
		float& OutDuration);
};
