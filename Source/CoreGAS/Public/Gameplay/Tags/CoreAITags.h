// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CoreGAS::AI
{
	// State tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Idle)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Patrol)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Combat_Movement)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Combat_Attack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Combat_TakeHit)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Death)

	// State event tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StateEvent_TargetDetected)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StateEvent_TakeHit)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_StateEvent_IncomingAttack)
}
