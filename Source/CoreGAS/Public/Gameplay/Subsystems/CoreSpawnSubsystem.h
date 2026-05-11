// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoreSpawnSubsystem.generated.h"

class UCoreASCBase;

USTRUCT(BlueprintType)
struct FHeroSpawnData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AActor> HeroActor;

	UPROPERTY()
	TObjectPtr<UCoreASCBase> HeroASC;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSpawn, const FHeroSpawnData&, HeroSpawnData);

UCLASS()
class COREGAS_API UCoreSpawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "CoreGAS|Spawn")
	FOnHeroSpawn OnHeroSpawn;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Spawn")
	void BroadcastHeroSpawn(const FHeroSpawnData& SpawnData);
};
