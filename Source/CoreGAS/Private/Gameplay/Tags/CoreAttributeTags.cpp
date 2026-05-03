// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Tags/CoreAttributeTags.h"

namespace CoreGAS::Attribute
{
	// Health state tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Full,  "CoreGAS.Attribute.Health.Full")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health_Empty, "CoreGAS.Attribute.Health.Empty")

	// Mana state tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Mana_Full,  "CoreGAS.Attribute.Mana.Full")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Mana_Empty, "CoreGAS.Attribute.Mana.Empty")

	// Stamina state tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Stamina_Full,  "CoreGAS.Attribute.Stamina.Full")
	UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Stamina_Empty, "CoreGAS.Attribute.Stamina.Empty")
}
