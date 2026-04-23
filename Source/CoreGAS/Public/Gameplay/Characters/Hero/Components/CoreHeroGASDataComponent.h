// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreHeroGASDataComponent.generated.h"

class UCoreHeroData;
class UCoreASCBase;
class UAttributeSet;

UCLASS()
class COREGAS_API UCoreHeroGASDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Data")
	TObjectPtr<UCoreHeroData> HeroData;

	void InitializeGASData(UCoreASCBase* ASC, UAttributeSet* AttributeSet);
};
