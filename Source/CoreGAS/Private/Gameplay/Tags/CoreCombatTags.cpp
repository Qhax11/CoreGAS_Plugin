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

	// Melee event tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Melee_TraceBegin, "CoreGAS.Event.Melee.TraceBegin")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Melee_TraceEnd,   "CoreGAS.Event.Melee.TraceEnd")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Melee_TraceBegin_RightHand, "CoreGAS.Event.Melee.TraceBegin.RightHand")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Melee_TraceBegin_LeftHand,  "CoreGAS.Event.Melee.TraceBegin.LeftHand")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Melee_TraceEnd_RightHand,   "CoreGAS.Event.Melee.TraceEnd.RightHand")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Melee_TraceEnd_LeftHand,    "CoreGAS.Event.Melee.TraceEnd.LeftHand")

	// Range event tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Range_Projectile_Spawn, "CoreGAS.Event.Range.Projectile.Spawn")

	// Death event tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Death, "CoreGAS.Event.Death")
}
