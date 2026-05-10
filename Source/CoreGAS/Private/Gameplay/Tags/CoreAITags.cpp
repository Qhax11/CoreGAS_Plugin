// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Tags/CoreAITags.h"

namespace CoreGAS::AI
{
	// State tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Idle,           "CoreGAS.AI.State.Idle")
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Patrol,         "CoreGAS.AI.State.Patrol")
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Combat_Movement,"CoreGAS.AI.State.Combat.Movement")
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Combat_Attack,  "CoreGAS.AI.State.Combat.Attack")
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Combat_TakeHit, "CoreGAS.AI.State.Combat.TakeHit")
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Death,          "CoreGAS.AI.State.Death")

	// State event tags
	UE_DEFINE_GAMEPLAY_TAG(TAG_StateEvent_TargetDetected, "CoreGAS.AI.StateEvent.TargetDetected")
	UE_DEFINE_GAMEPLAY_TAG(TAG_StateEvent_TakeHit,        "CoreGAS.AI.StateEvent.TakeHit")
	UE_DEFINE_GAMEPLAY_TAG(TAG_StateEvent_IncomingAttack, "CoreGAS.AI.StateEvent.IncomingAttack")
}
