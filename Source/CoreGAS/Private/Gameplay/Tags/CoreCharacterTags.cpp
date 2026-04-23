// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Tags/CoreCharacterTags.h"

namespace CoreGAS::Character
{
	// Status tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Dead,       "CoreGAS.Status.Dead")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Stunned,    "CoreGAS.Status.Stunned")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Status_Invincible, "CoreGAS.Status.Invincible")

	// State tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_InCombat, "CoreGAS.State.InCombat")
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_InAir,    "CoreGAS.State.InAir")

	// Entity tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Entity_Hero,  "CoreGAS.Entity.Hero")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Entity_Enemy, "CoreGAS.Entity.Enemy")
}
