// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Animation/Notifies/CoreAnimNotifyState_PhaseTag.h"
#include "Gameplay/Tags/CoreAbilityTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UCoreAnimNotifyState_PhaseTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_PhaseTag::NotifyBegin - No owner actor found on MeshComp."));
		return;
	}

	if (!PhaseTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_PhaseTag::NotifyBegin - PhaseTag is not valid on %s."), *Owner->GetName());
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_PhaseTag::NotifyBegin - No ASC found on %s."), *Owner->GetName());
		return;
	}

	ASC->AddLooseGameplayTag(PhaseTag);
}

void UCoreAnimNotifyState_PhaseTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_PhaseTag::NotifyEnd - No owner actor found on MeshComp."));
		return;
	}

	if (!PhaseTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_PhaseTag::NotifyEnd - PhaseTag is not valid on %s."), *Owner->GetName());
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_PhaseTag::NotifyEnd - No ASC found on %s."), *Owner->GetName());
		return;
	}

	ASC->RemoveLooseGameplayTag(PhaseTag);
}

FString UCoreAnimNotifyState_PhaseTag::GetNotifyName_Implementation() const
{
	if (PhaseTag.IsValid())
	{
		return FString::Printf(TEXT("PhaseTag: %s"), *PhaseTag.ToString());
	}

	return TEXT("PhaseTag: None");
}
