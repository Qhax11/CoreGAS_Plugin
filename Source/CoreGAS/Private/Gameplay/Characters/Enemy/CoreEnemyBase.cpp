// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Enemy/CoreEnemyBase.h"
#include "Gameplay/Characters/Enemy/Components/CoreEnemyGASDataComponent.h"
#include "Gameplay/Attributes/Enemy/CoreEnemyAttributeSet.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "Gameplay/AI/CoreAIController.h"
#include "Gameplay/AI/Spawning/CoreEnemyRowData.h"

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
}

void ACoreEnemyBase::InitializeGAS()
{
	EnemyGASDataComponent->InitializeGASData(AbilitySystemComponent, EnemyAttributeSet);
}

void ACoreEnemyBase::InitializeFromData(const FCoreEnemyRowData& RowData)
{
	GetMesh()->SetSkeletalMesh(RowData.Mesh.LoadSynchronous());
	GetMesh()->SetAnimInstanceClass(RowData.AnimBP);
	GetMesh()->SetRelativeLocation(RowData.MeshOffset);
	GetMesh()->SetRelativeRotation(RowData.MeshRotation);

	if (ACoreAIController* EnemyController = Cast<ACoreAIController>(GetController()))
	{
		EnemyController->InitializeBehavior(RowData.ArchetypeData);
	}
}
