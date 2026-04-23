// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Hero/Components/CoreHeroInputComponent.h"
#include "Gameplay/Components/CoreASCBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UCoreHeroInputComponent::InitializeInputComponent(UCoreASCBase* ASC, UInputComponent* InputComponent, APlayerController* PC)
{
	CachedASC = ASC;
	CachedInputComponent = InputComponent;

	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, MappingPriority);
			}
		}
	}
}

void UCoreHeroInputComponent::BindAbilityInput(UInputAction* Action, const FGameplayTag& InputTag)
{
	if (!Action || !CachedASC || !CachedInputComponent)
	{
		return;
	}

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(CachedInputComponent);
	EIC->BindAction(Action, ETriggerEvent::Started,   CachedASC.Get(), &UCoreASCBase::AbilityInputTagPressed,  InputTag);
	EIC->BindAction(Action, ETriggerEvent::Completed, CachedASC.Get(), &UCoreASCBase::AbilityInputTagReleased, InputTag);
}
