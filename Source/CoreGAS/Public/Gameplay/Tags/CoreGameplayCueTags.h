// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CoreGAS::GameplayCue
{
	// HitReaction cue tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_HitReaction_Flash)

	// Impact cue tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Impact_Melee)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Impact_MeleeAoE)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Impact_Block)
}
