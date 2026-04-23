// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Tags/CoreCombatTags.h"

namespace CoreGAS::Combat
{
	// Ability tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack, "CoreGAS.Ability.Attack")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Jump,   "CoreGAS.Ability.Jump")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Dodge,  "CoreGAS.Ability.Dodge")

	// Effect tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Damage, "CoreGAS.Effect.Damage")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Heal,   "CoreGAS.Effect.Heal")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_DoT,    "CoreGAS.Effect.DoT")

	// SetByCaller tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_SetByCaller_DamageAmount, "CoreGAS.SetByCaller.DamageAmount")
	UE_DEFINE_GAMEPLAY_TAG(TAG_SetByCaller_HealAmount,   "CoreGAS.SetByCaller.HealAmount")
}
