// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/CoreCharacterBase.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Components/CoreEquipmentComponent.h"

ACoreCharacterBase::ACoreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UCoreASCBase>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);

	EquipmentComponent = CreateDefaultSubobject<UCoreEquipmentComponent>(TEXT("EquipmentComponent"));
}

UAbilitySystemComponent* ACoreCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACoreCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeGAS();
}
