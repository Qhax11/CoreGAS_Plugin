// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CoreGAS::Character
{
	// Status tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Dead)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Stunned)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Invincible)

	// State tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_InCombat)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_InAir)

	// Entity tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Entity_Hero)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Entity_Enemy)
}
