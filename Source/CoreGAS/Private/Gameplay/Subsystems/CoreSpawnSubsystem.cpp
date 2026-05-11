// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Subsystems/CoreSpawnSubsystem.h"

void UCoreSpawnSubsystem::BroadcastHeroSpawn(const FHeroSpawnData& SpawnData)
{
	OnHeroSpawn.Broadcast(SpawnData);
}
