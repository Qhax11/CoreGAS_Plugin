// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/CoreCharacterBase.h"
#include "CoreEnemyBase.generated.h"

class UCoreEnemyGASDataComponent;
class UCoreEnemyAttributeSet;
class ACoreAIController;
struct FCoreEnemyRowData;

UCLASS()
class COREGAS_API ACoreEnemyBase : public ACoreCharacterBase
{
	GENERATED_BODY()

public:
	ACoreEnemyBase();
	virtual UCoreASCBase* GetCoreAbilitySystemComponent() const override;
	void InitializeFromData(const FCoreEnemyRowData& RowData);

protected:
	virtual void InitializeGAS() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreEnemyGASDataComponent> EnemyGASDataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Attributes")
	TObjectPtr<UCoreEnemyAttributeSet> EnemyAttributeSet;
};
