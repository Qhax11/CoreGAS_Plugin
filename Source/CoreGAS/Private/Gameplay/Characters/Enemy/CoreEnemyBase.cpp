// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Enemy/CoreEnemyBase.h"
#include "Gameplay/Characters/Enemy/Components/CoreEnemyGASDataComponent.h"
#include "Gameplay/Attributes/Enemy/CoreEnemyAttributeSet.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "Gameplay/AI/CoreAIController.h"

UCoreASCBase* ACoreEnemyBase::GetCoreAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

ACoreEnemyBase::ACoreEnemyBase()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECC_Enemy);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Damage, ECR_Block);

	EnemyGASDataComponent = CreateDefaultSubobject<UCoreEnemyGASDataComponent>(TEXT("EnemyGASDataComponent"));
	EnemyAttributeSet     = CreateDefaultSubobject<UCoreEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	AIControllerClass = ACoreAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACoreEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyGASDataComponent->InitializeGASData(AbilitySystemComponent, EnemyAttributeSet);
}