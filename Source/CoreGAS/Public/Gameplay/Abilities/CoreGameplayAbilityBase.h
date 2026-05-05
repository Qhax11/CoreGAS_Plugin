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
	UCoreGameplayAbilityBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Ability", Meta = (Categories = "CoreGAS.Input"))
	FGameplayTag AbilityInputTag;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Commit")
	bool bCommitCooldownOnActivate = true;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Commit")
	bool bCommitCostOnActivate = true;
};
