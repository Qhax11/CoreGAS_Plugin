// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Actors/Weapon/CoreWeaponBase.h"
#include "CoreWeaponSlotData.generated.h"

USTRUCT(BlueprintType)
struct FCoreWeaponSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Equipment")
	TSubclassOf<ACoreWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Equipment")
	FName AttachSocket = "SOC_hand_r";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Equipment", Meta = (Categories = "CoreGAS.Equipment.Slot"))
	FGameplayTag SlotTag;
};
