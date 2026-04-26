// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbility_Montage.h"
#include "CoreAbility_Death.generated.h"

UCLASS()
class COREGAS_API UCoreAbility_Death : public UCoreGameplayAbility_Montage
{
	GENERATED_BODY()

public:
	UCoreAbility_Death();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
