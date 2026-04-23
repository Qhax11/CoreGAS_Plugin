// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreEnemyGASDataComponent.generated.h"

class UCoreCharacterData;
class UCoreASCBase;
class UAttributeSet;

UCLASS()
class COREGAS_API UCoreEnemyGASDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Data")
	TObjectPtr<UCoreCharacterData> CharacterData;

	void InitializeGASData(UCoreASCBase* ASC, UAttributeSet* AttributeSet);
};
