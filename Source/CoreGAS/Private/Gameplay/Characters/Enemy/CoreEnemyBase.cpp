// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Enemy/CoreEnemyBase.h"
#include "Gameplay/Characters/Enemy/Components/CoreEnemyGASDataComponent.h"
#include "Gameplay/Attributes/Enemy/CoreEnemyAttributeSet.h"
#include "Gameplay/Components/CoreASCBase.h"

UCoreASCBase* ACoreEnemyBase::GetCoreAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

ACoreEnemyBase::ACoreEnemyBase()
{
	EnemyGASDataComponent = CreateDefaultSubobject<UCoreEnemyGASDataComponent>(TEXT("EnemyGASDataComponent"));
	EnemyAttributeSet = CreateDefaultSubobject<UCoreEnemyAttributeSet>(TEXT("EnemyAttributeSet"));
}

void ACoreEnemyBase::InitializeGAS()
{
	EnemyGASDataComponent->InitializeGASData(AbilitySystemComponent, EnemyAttributeSet);
}
