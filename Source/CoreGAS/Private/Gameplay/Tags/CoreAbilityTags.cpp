// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Tags/CoreAbilityTags.h"

namespace CoreGAS::Ability
{
	// Phase tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Phase_Startup,  "CoreGAS.Ability.Phase.Startup")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Phase_Active,   "CoreGAS.Ability.Phase.Active")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Phase_Recovery, "CoreGAS.Ability.Phase.Recovery")
}
