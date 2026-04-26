// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CoreEquipmentData.generated.h"

class ACoreWeaponBase;

UCLASS(BlueprintType)
class COREGAS_API UCoreEquipmentData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Equipment")
	TMap<FName, TSubclassOf<ACoreWeaponBase>> DefaultWeapons;
};
