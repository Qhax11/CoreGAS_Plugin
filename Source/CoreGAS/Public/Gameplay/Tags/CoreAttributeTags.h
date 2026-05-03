// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CoreGAS::Attribute
{
	// Health state tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Full)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health_Empty)

	// Mana state tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Mana_Full)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Mana_Empty)

	// Stamina state tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Stamina_Full)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Stamina_Empty)
}
