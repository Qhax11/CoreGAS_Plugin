// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/CoreCharacterBase.h"
#include "CoreEnemyBase.generated.h"

class UCoreEnemyGASDataComponent;
class UCoreEnemyAttributeSet;
class ACoreAIController;

UCLASS()
class COREGAS_API ACoreEnemyBase : public ACoreCharacterBase
{
	GENERATED_BODY()

public:
	ACoreEnemyBase();
	virtual UCoreASCBase* GetCoreAbilitySystemComponent() const override;

protected:
	virtual void InitializeGAS() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreEnemyGASDataComponent> EnemyGASDataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Attributes")
	TObjectPtr<UCoreEnemyAttributeSet> EnemyAttributeSet;
};
