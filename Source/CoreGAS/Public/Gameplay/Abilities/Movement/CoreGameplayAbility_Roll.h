// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbility_Montage.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "GameplayTagContainer.h"
#include "CoreGameplayAbility_Roll.generated.h"

class UAbilityTask_WaitDelay;

USTRUCT(BlueprintType)
struct FCoreRollMontageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MotionWarpingDistance = 300.f;
};

USTRUCT(BlueprintType)
struct FCoreDirectionRollPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "CoreGAS.Ability.Direction"))
	FGameplayTag DirectionTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCoreRollMontageData MontageData;
};

UCLASS(BlueprintType)
class COREGAS_API UCoreRollDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FCoreDirectionRollPair> DirectionRollPairs;

	UAnimMontage* FindMontage(FGameplayTag DirectionTag) const;
	float FindWarpDistance(FGameplayTag DirectionTag) const;
};

UCLASS()
class COREGAS_API UCoreGameplayAbility_Roll : public UCoreGameplayAbility_Montage
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_Roll();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Roll")
	UCoreRollDataAsset* RollDataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Roll")
	float DamageImmunityDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Roll")
	FName WarpTargetName = "RollTarget";

	UFUNCTION()
	void RemoveDamageImmunity();

	FGameplayTag GetDirectionTagFromInput(const FVector2D& Input) const;

	UPROPERTY()
	UAbilityTask_WaitDelay* WaitDelayTask = nullptr;

	FVector2D CachedInputDirection;

	FVector CachedWorldDashDir = FVector::ZeroVector;
};
