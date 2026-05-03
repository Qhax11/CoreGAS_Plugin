// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Cues/CoreGameplayCueNotify_Actor.h"

bool ACoreGameplayCueNotify_Actor::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	Super::OnExecute_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	return OnExecuted(Source, MyTarget, Parameters);
}

bool ACoreGameplayCueNotify_Actor::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	bool bResult = Super::OnActive_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	OnActivated(Source, MyTarget, Parameters);
	return bResult;
}

bool ACoreGameplayCueNotify_Actor::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	bool bResult = Super::WhileActive_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	OnWhileActivated(Source, MyTarget, Parameters);
	return bResult;
}

bool ACoreGameplayCueNotify_Actor::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	bool bResult = Super::OnRemove_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	OnRemoved(Source, MyTarget, Parameters);
	return bResult;
}

bool ACoreGameplayCueNotify_Actor::OnExecuted_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	// Logic will be implemented in subclasses.
	return false;
}

void ACoreGameplayCueNotify_Actor::OnActivated_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	// Logic will be implemented in subclasses.
}

void ACoreGameplayCueNotify_Actor::OnWhileActivated_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	// Logic will be implemented in subclasses.
}

void ACoreGameplayCueNotify_Actor::OnRemoved_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	// Logic will be implemented in subclasses.
}
