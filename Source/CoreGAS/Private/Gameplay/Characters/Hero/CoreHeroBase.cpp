// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Hero/CoreHeroBase.h"
#include "Gameplay/Characters/Hero/Components/CoreHeroInputComponent.h"
#include "Gameplay/Characters/Hero/Components/CoreHeroDataComponent.h"
#include "Gameplay/Components/CoreTargetingComponent.h"
#include "Gameplay/Attributes/Hero/CoreHeroAttributeSet.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "Gameplay/Data/CoreHeroGASData.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "Gameplay/Subsystems/CoreSpawnSubsystem.h"

UCoreASCBase* ACoreHeroBase::GetCoreAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

AActor* ACoreHeroBase::GetCurrentTarget() const
{
	return TargetingComponent ? TargetingComponent->GetCurrentTarget() : nullptr;
}

ACoreHeroBase::ACoreHeroBase()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECC_Hero);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Damage, ECR_Block);

	HeroInputComponent    = CreateDefaultSubobject<UCoreHeroInputComponent>(TEXT("HeroInputComponent"));
	HeroDataComponent = CreateDefaultSubobject<UCoreHeroDataComponent>(TEXT("HeroDataComponent"));
	HeroAttributeSet      = CreateDefaultSubobject<UCoreHeroAttributeSet>(TEXT("HeroAttributeSet"));
	TargetingComponent    = CreateDefaultSubobject<UCoreTargetingComponent>(TEXT("TargetingComponent"));
}

void ACoreHeroBase::BeginPlay()
{
	Super::BeginPlay();

	HeroDataComponent->Initialize(AbilitySystemComponent, HeroAttributeSet);

	UCoreSpawnSubsystem* SpawnSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCoreSpawnSubsystem>();
	if (SpawnSubsystem)
	{
		FHeroSpawnData SpawnData;
		SpawnData.HeroActor = this;
		SpawnData.HeroASC   = AbilitySystemComponent;
		SpawnSubsystem->BroadcastHeroSpawn(SpawnData);
	}
}

void ACoreHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	HeroInputComponent->InitializeInputComponent(AbilitySystemComponent, PlayerInputComponent, Cast<APlayerController>(GetController()));

	if (!HeroDataComponent->GASData)
	{
		return;
	}

	for (const FCoreAbilityInputBinding& Binding : HeroDataComponent->GASData->AbilityInputBindings)
	{
		if (UInputAction* Action = Binding.InputAction.LoadSynchronous())
		{
			HeroInputComponent->BindAbilityInput(Action, Binding.InputTag);
		}
	}
}
