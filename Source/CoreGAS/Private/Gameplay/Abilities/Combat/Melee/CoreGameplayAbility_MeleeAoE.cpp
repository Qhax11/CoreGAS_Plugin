// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Abilities/Combat/Melee/CoreGameplayAbility_MeleeAoE.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "Gameplay/Tags/CoreGameplayCueTags.h"
#include "Gameplay/Tracing/CoreTraceConfig.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/DecalComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

UCoreGameplayAbility_MeleeAoE::UCoreGameplayAbility_MeleeAoE()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_AoE_Impact);
}

void UCoreGameplayAbility_MeleeAoE::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!AoETraceConfig)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (AoEDecalMaterial)
	{
		AActor* AvatarActor = GetAvatarActorFromActorInfo();
		const FVector SocketLocation = AoETraceConfig->GetStartLocation(AvatarActor, FVector::ZeroVector);

		FHitResult GroundHit;
		GetWorld()->LineTraceSingleByChannel(
			GroundHit,
			SocketLocation,
			SocketLocation + FVector::DownVector * 300.f,
			ECC_WorldStatic
		);

		const FVector DecalLocation = GroundHit.bBlockingHit ? GroundHit.ImpactPoint : SocketLocation;
		const FRotator DecalRotation = FRotator(-90.f, 0.f, 0.f); // yere dik

		const float AoERadius = AoETraceConfig->ShapeRadius;
		ActiveDecal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			AoEDecalMaterial,
			FVector(5.f, AoERadius, AoERadius),
			DecalLocation,
			DecalRotation
		);

		if (ActiveDecal)
		{
			ActiveDecal->SetFadeOut(0.f, 0.f, false); // impact'te manuel destroy edeceğiz
		}
	}
}

void UCoreGameplayAbility_MeleeAoE::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag != CoreGAS::Combat::TAG_Event_AoE_Impact)
	{
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	TArray<FHitResult> HitResults = AoETraceConfig->Execute(AvatarActor, FVector::ZeroVector, FVector::ZeroVector, AvatarActor);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		return;
	}

	if (!HitEffects.IsEmpty())
	{
		const bool bSourceIsEnemy = SourceASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Entity_Enemy);
		TSet<AActor*> HitActors;

		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor || HitActors.Contains(HitActor))
			{
				continue;
			}

			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if (!TargetASC)
			{
				continue;
			}

			if (bSourceIsEnemy && TargetASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Entity_Enemy))
			{
				continue;
			}

			ApplyHitToTarget(SourceASC, TargetASC, Hit);

			HitActors.Add(HitActor);
		}
	}

	const FVector SocketLocation = AoETraceConfig->GetStartLocation(AvatarActor, FVector::ZeroVector);

	FHitResult GroundHit;
	GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		SocketLocation,
		SocketLocation + FVector::DownVector * 300.f,
		ECC_WorldStatic
	);

	FGameplayCueParameters CueParams;
	CueParams.Location = GroundHit.bBlockingHit ? GroundHit.ImpactPoint : SocketLocation;
	CueParams.Normal = GroundHit.bBlockingHit ? GroundHit.ImpactNormal : FVector::UpVector;
	SourceASC->ExecuteGameplayCue(CoreGAS::GameplayCue::TAG_GameplayCue_Impact_MeleeAoE, CueParams);
}

void UCoreGameplayAbility_MeleeAoE::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(ActiveDecal)) 
	{
		ActiveDecal->DestroyComponent();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
