// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Gameplay/AI/BehaviorDecision/Data/CoreEnemyArchetypeData.h"
#include "Gameplay/Data/CoreCharacterData.h"
#include "CoreEnemyRowData.generated.h"

USTRUCT(BlueprintType)
struct FCoreEnemyRowData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EnemyName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCoreCharacterData> CharacterData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCoreEnemyArchetypeData> ArchetypeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector MeshOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator MeshRotation = FRotator::ZeroRotator;

};
