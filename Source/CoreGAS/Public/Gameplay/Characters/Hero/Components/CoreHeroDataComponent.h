// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreHeroDataComponent.generated.h"

class UCoreHeroGASData;
class UCoreASCBase;
class UAttributeSet;

UCLASS()
class COREGAS_API UCoreHeroDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Data")
	TObjectPtr<UCoreHeroGASData> GASData;

	void Initialize(UCoreASCBase* ASC, UAttributeSet* AttributeSet);
};
