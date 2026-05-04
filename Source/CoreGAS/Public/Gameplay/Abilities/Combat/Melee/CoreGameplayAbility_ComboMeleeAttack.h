// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/Combat/Melee/CoreGameplayAbility_MeleeAttack.h"
#include "CoreGameplayAbility_ComboMeleeAttack.generated.h"

UCLASS()
class COREGAS_API UCoreGameplayAbility_ComboMeleeAttack : public UCoreGameplayAbility_MeleeAttack
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_ComboMeleeAttack();
};
