// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Attributes/CoreAttributeSetBase.h"
#include "CoreGASTypes.generated.h"

// Collision Object Channels - must match DefaultEngine.ini channel assignments
#define ECC_Hero    ECC_GameTraceChannel1
#define ECC_Enemy   ECC_GameTraceChannel2

// Collision Trace Channels
#define ECC_Damage  ECC_GameTraceChannel3

USTRUCT(BlueprintType)
struct FCoreAttributeChangeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	FGameplayAttribute Attribute;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	float NewValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	float OldValue = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoreOnAttributeChanged, FCoreAttributeChangeData, ChangeData);

UENUM(BlueprintType)
enum class ECoreTagChangeEvent : uint8
{
	Added,        // 0 -> 1
	Removed,      // 1 -> 0
	StackAdded,   // n -> n+1
	StackRemoved  // n -> n-1
};

USTRUCT(BlueprintType)
struct FCoreTagChangeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	int32 OldCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	ECoreTagChangeEvent ChangeEvent = ECoreTagChangeEvent::Added;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FCoreOnTagChanged, const FCoreTagChangeData&, ChangeData);

USTRUCT(BlueprintType)
struct FCoreDamageCalculationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	float RawDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	float MitigatedDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS")
	float DamageDealt = 0.f;
};

USTRUCT()
struct COREGAS_API FCoreExecCalculationParameters
{
	GENERATED_BODY()

public:
	FCoreExecCalculationParameters() = default;
	explicit FCoreExecCalculationParameters(const FGameplayEffectCustomExecutionParameters& InParams);

	AActor* SourceActor = nullptr;
	UCoreASCBase* SourceASC = nullptr;
	AActor* TargetActor = nullptr;
	UCoreASCBase* TargetASC = nullptr;
	FGameplayTagContainer EffectAssetTags;
	FGameplayEffectSpec* MutableSpec = nullptr;
	const FGameplayEffectCustomExecutionParameters* ExecutionParams = nullptr;
	const FHitResult* HitResult = nullptr;

	const UCoreAttributeSetBase* GetSourceAttributeSet() const;
	const UCoreAttributeSetBase* GetTargetAttributeSet() const;
	const FGameplayEffectSpec& GetSpec() const;
	const FHitResult* GetHitResult() const;

	template<typename T>
	const T* GetSourceAttributeSetAs() const { return SourceASC ? SourceASC->GetSet<T>() : nullptr; }

	template<typename T>
	const T* GetTargetAttributeSetAs() const { return TargetASC ? TargetASC->GetSet<T>() : nullptr; }

	template<typename T>
	const T* GetSourceActorAs() const { return Cast<T>(SourceActor); }

	template<typename T>
	const T* GetTargetActorAs() const { return Cast<T>(TargetActor); }
};
