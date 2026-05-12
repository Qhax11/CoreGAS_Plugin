// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Gameplay/AI/Spawning/CoreEnemyRowData.h"
#include "CoreEnemySpawner.generated.h"

class ACoreEnemyBase;

USTRUCT(BlueprintType)
struct FCoreEnemySpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName RowName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FTransform SpawnTransform;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 Count = 1;
};

UCLASS()
class COREGAS_API ACoreEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	ACoreEnemySpawner();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "CoreGAS|Spawning", meta = (RequiredAssetDataTags = "RowStructure=/Script/CoreGAS.CoreEnemyRowData"))
	TObjectPtr<UDataTable> EnemyDataTable;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "CoreGAS|Spawning")
	TArray<FCoreEnemySpawnEntry> SpawnEntries;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "CoreGAS|Spawning")
	bool bSpawnOnBeginPlay = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "CoreGAS|Spawning")
	TSubclassOf<ACoreEnemyBase> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Spawning|Debug")
	bool bEnableDebug = false;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Spawning")
	void SpawnAll();

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Spawning")
	void SpawnEntry(const FCoreEnemySpawnEntry& Entry);

protected:
	virtual void BeginPlay() override;
};
