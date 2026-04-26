// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CoreGAS::Combat
{
	// Ability tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Jump)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Dodge)

	// Effect tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Heal)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_DoT)

	// SetByCaller tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SetByCaller_DamageAmount)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SetByCaller_HealAmount)

	// Melee event tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Melee_TraceBegin)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Melee_TraceEnd)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Melee_TraceBegin_RightHand)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Melee_TraceBegin_LeftHand)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Melee_TraceEnd_RightHand)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Melee_TraceEnd_LeftHand)
}
