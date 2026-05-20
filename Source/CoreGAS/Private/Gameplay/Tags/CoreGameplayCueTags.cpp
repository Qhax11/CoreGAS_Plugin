// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Tags/CoreGameplayCueTags.h"

namespace CoreGAS::GameplayCue
{
	// HitReaction cue tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_HitReaction_Flash, "GameplayCue.HitReaction.Flash")

	// Impact cue tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Impact_Melee, "GameplayCue.Impact.Melee")
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Impact_MeleeAoE, "GameplayCue.Impact.MeleeAoE")
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Impact_Block, "GameplayCue.Impact.Block")
}
