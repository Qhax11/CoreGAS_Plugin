// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Components/CoreTargetingComponent.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"

UCoreTargetingComponent::UCoreTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoreTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		TargetingTimerHandle,
		this,
		&UCoreTargetingComponent::UpdateTarget,
		TargetingInterval,
		true
	);
}

void UCoreTargetingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TargetingTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void UCoreTargetingComponent::UpdateTarget()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	APlayerController* PC = Owner->GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	const FVector OwnerLocation = Owner->GetActorLocation();

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_Pawn) };
	const TArray<AActor*> ActorsToIgnore = { Owner };
	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(this, OwnerLocation, TargetingRadius, ObjectTypes, nullptr, ActorsToIgnore, OverlappedActors);

	if (bDrawDebug)
	{
		DrawDebugSphere(Owner->GetWorld(), OwnerLocation, TargetingRadius, 16, FColor::Green, false, TargetingInterval);
	}

	int32 SizeX, SizeY;
	PC->GetViewportSize(SizeX, SizeY);
	const FVector2D ScreenCenter(SizeX * 0.5f, SizeY * 0.5f);

	AActor* BestCandidate = nullptr;
	float   BestScore     = -MAX_FLT;

	for (AActor* Candidate : OverlappedActors)
	{
		if (!Candidate || !Candidate->FindComponentByClass<UAbilitySystemComponent>())
		{
			continue;
		}

		FVector2D ScreenPos;
		if (!PC->ProjectWorldLocationToScreen(Candidate->GetActorLocation(), ScreenPos, false))
		{
			continue;
		}

		const float ScreenDistance = FVector2D::Distance(ScreenPos, ScreenCenter);
		const float WorldDistance  = FVector::Dist(OwnerLocation, Candidate->GetActorLocation());
		const float Score = (1.0f / FMath::Max(ScreenDistance, 1.f)) * 0.7f
		                  + (1.0f / FMath::Max(WorldDistance,  1.f)) * 0.3f;

		if (Score > BestScore)
		{
			BestScore     = Score;
			BestCandidate = Candidate;
		}
	}

	if (BestCandidate == CurrentTarget)
	{
		return;
	}

	if (IsValid(CurrentTarget))
	{
		if (UAbilitySystemComponent* OldASC = CurrentTarget->FindComponentByClass<UAbilitySystemComponent>())
		{
			OldASC->RemoveLooseGameplayTag(CoreGAS::Character::TAG_State_Targeted);
		}
	}

	CurrentTarget = BestCandidate;

	if (IsValid(CurrentTarget))
	{
		if (UAbilitySystemComponent* NewASC = CurrentTarget->FindComponentByClass<UAbilitySystemComponent>())
		{
			NewASC->AddLooseGameplayTag(CoreGAS::Character::TAG_State_Targeted);
		}
	}
}
