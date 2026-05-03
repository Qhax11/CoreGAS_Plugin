// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Cues/CoreGameplayCueNotify_Static.h"

bool UCoreGameplayCueNotify_Static::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	Super::OnExecute_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	return OnExecuted(Source, MyTarget, Parameters);
}

bool UCoreGameplayCueNotify_Static::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	bool bResult = Super::OnActive_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	OnActivated(Source, MyTarget, Parameters);
	return bResult;
}

bool UCoreGameplayCueNotify_Static::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	bool bResult = Super::WhileActive_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	OnWhileActivated(Source, MyTarget, Parameters);
	return bResult;
}

bool UCoreGameplayCueNotify_Static::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	bool bResult = Super::OnRemove_Implementation(MyTarget, Parameters);
	AActor* Source = Parameters.GetEffectCauser();
	OnRemoved(Source, MyTarget, Parameters);
	return bResult;
}

bool UCoreGameplayCueNotify_Static::OnExecuted_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const
{
	// Logic will be implemented in subclasses.
	return false;
}

void UCoreGameplayCueNotify_Static::OnActivated_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const
{
	// Logic will be implemented in subclasses.
}

void UCoreGameplayCueNotify_Static::OnWhileActivated_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const
{
	// Logic will be implemented in subclasses.
}

void UCoreGameplayCueNotify_Static::OnRemoved_Implementation(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const
{
	// Logic will be implemented in subclasses.
}
