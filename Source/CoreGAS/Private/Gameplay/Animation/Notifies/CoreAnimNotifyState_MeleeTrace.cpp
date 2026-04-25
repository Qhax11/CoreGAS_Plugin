// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Animation/Notifies/CoreAnimNotifyState_MeleeTrace.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"

UCoreAnimNotifyState_MeleeTrace::UCoreAnimNotifyState_MeleeTrace()
{
	TraceBeginTag = CoreGAS::Combat::TAG_Event_Melee_TraceBegin;
	TraceEndTag = CoreGAS::Combat::TAG_Event_Melee_TraceEnd;
}

void UCoreAnimNotifyState_MeleeTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_MeleeTrace::NotifyBegin - No owner actor found on MeshComp."));
		return;
	}

	if (!TraceBeginTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_MeleeTrace::NotifyBegin - TraceBeginTag is not valid on %s."), *Owner->GetName());
		return;
	}

	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_MeleeTrace::NotifyBegin - No ASC found on %s."), *Owner->GetName());
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, TraceBeginTag, FGameplayEventData());
}

void UCoreAnimNotifyState_MeleeTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_MeleeTrace::NotifyEnd - No owner actor found on MeshComp."));
		return;
	}

	if (!TraceEndTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_MeleeTrace::NotifyEnd - TraceEndTag is not valid on %s."), *Owner->GetName());
		return;
	}

	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotifyState_MeleeTrace::NotifyEnd - No ASC found on %s."), *Owner->GetName());
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, TraceEndTag, FGameplayEventData());
}
