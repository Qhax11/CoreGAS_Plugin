// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Animation/Notifies/CoreAnimNotify_SendEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UCoreAnimNotify_SendEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotify_SendEvent::Notify - No owner actor found on MeshComp."));
		return;
	}

	if (!EventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotify_SendEvent::Notify - EventTag is not valid on %s."), *Owner->GetName());
		return;
	}

	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreAnimNotify_SendEvent::Notify - No ASC found on %s."), *Owner->GetName());
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());
}

FString UCoreAnimNotify_SendEvent::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		return FString::Printf(TEXT("SendEvent: %s"), *EventTag.ToString());
	}

	return TEXT("SendEvent: None");
}
