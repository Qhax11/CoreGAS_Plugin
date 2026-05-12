// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/AI/Spawning/CoreEnemySpawner.h"
#include "Gameplay/Characters/Enemy/CoreEnemyBase.h"

ACoreEnemySpawner::ACoreEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACoreEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnOnBeginPlay)
	{
		SpawnAll();
	}
}

void ACoreEnemySpawner::SpawnAll()
{
	for (const FCoreEnemySpawnEntry& Entry : SpawnEntries)
	{
		SpawnEntry(Entry);
	}
}

void ACoreEnemySpawner::SpawnEntry(const FCoreEnemySpawnEntry& Entry)
{
	if (!EnemyDataTable || Entry.RowName.IsNone())
	{
		return;
	}

	const FString Context = TEXT("ACoreEnemySpawner::SpawnEntry");
	const FCoreEnemyRowData* RowData = EnemyDataTable->FindRow<FCoreEnemyRowData>(Entry.RowName, Context);

	if (!RowData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CoreEnemySpawner] Row '%s' not found in DataTable."), *Entry.RowName.ToString());
		return;
	}

	for (int32 i = 0; i < Entry.Count; ++i)
	{
		ACoreEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<ACoreEnemyBase>(EnemyClass, Entry.SpawnTransform);
		if (!SpawnedEnemy)
		{
			continue;
		}

		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Log, TEXT("[CoreEnemySpawner] Spawned '%s' at %s"),
				*RowData->EnemyName.ToString(),
				*Entry.SpawnTransform.GetLocation().ToString());
		}

		SpawnedEnemy->InitializeFromData(*RowData);
	}
}
