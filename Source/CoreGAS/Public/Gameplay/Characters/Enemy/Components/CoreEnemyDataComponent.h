// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreEnemyDataComponent.generated.h"

class UCoreGASData;
class UCoreEnemyAIData;
class UCoreASCBase;
class UAttributeSet;

UCLASS()
class COREGAS_API UCoreEnemyDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Data")
	TObjectPtr<UCoreGASData> GASData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|AI")
	TObjectPtr<UCoreEnemyAIData> AIData;

	void Initialize(UCoreASCBase* ASC, UAttributeSet* AttributeSet);
};
