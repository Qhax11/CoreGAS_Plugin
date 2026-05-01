// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CoreGAS::Ability
{
	// Phase tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Phase_Startup)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Phase_Active)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Phase_Recovery)
}
