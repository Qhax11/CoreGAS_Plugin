// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Hero/CoreHeroBase.h"
#include "Gameplay/Characters/Hero/Components/CoreHeroInputComponent.h"
#include "Gameplay/Characters/Hero/Components/CoreHeroGASDataComponent.h"
#include "Gameplay/Components/CoreTargetingComponent.h"
#include "Gameplay/Attributes/Hero/CoreHeroAttributeSet.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Data/CoreHeroData.h"
#include "Gameplay/Types/CoreGASTypes.h"

UCoreASCBase* ACoreHeroBase::GetCoreAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

ACoreHeroBase::ACoreHeroBase()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECC_Hero);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Damage, ECR_Block);

	HeroInputComponent    = CreateDefaultSubobject<UCoreHeroInputComponent>(TEXT("HeroInputComponent"));
	HeroGASDataComponent  = CreateDefaultSubobject<UCoreHeroGASDataComponent>(TEXT("HeroGASDataComponent"));
	HeroAttributeSet      = CreateDefaultSubobject<UCoreHeroAttributeSet>(TEXT("HeroAttributeSet"));
	TargetingComponent    = CreateDefaultSubobject<UCoreTargetingComponent>(TEXT("TargetingComponent"));
}

void ACoreHeroBase::InitializeGAS()
{
	HeroGASDataComponent->InitializeGASData(AbilitySystemComponent, HeroAttributeSet);
}

void ACoreHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	HeroInputComponent->InitializeInputComponent(AbilitySystemComponent, PlayerInputComponent, Cast<APlayerController>(GetController()));

	if (!HeroGASDataComponent->HeroData)
	{
		return;
	}

	for (const FCoreAbilityInputBinding& Binding : HeroGASDataComponent->HeroData->AbilityInputBindings)
	{
		if (UInputAction* Action = Binding.InputAction.LoadSynchronous())
		{
			HeroInputComponent->BindAbilityInput(Action, Binding.InputTag);
		}
	}
}
