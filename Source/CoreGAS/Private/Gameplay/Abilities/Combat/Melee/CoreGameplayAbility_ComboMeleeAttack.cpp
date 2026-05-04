// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Combat/Melee/CoreGameplayAbility_ComboMeleeAttack.h"
#include "Gameplay/Tags/CoreCombatTags.h"

UCoreGameplayAbility_ComboMeleeAttack::UCoreGameplayAbility_ComboMeleeAttack()
{
	ActivationOwnedTags.AddTag(CoreGAS::Combat::TAG_State_Combo_Active);
}
