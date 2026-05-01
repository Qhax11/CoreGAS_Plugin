// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Abilities/Combat/Ranged/CoreGameplayAbility_Projectile.h"
#include "Gameplay/Actors/CoreProjectile.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"

UCoreGameplayAbility_Projectile::UCoreGameplayAbility_Projectile()
{
	WaitForEventTags.AddTag(CoreGAS::Combat::TAG_Event_Range_Projectile_Spawn);
}

void UCoreGameplayAbility_Projectile::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(AvatarActor);
	USkeletalMeshComponent* Mesh = Character ? Character->GetMesh() : nullptr;
	const FVector SpawnLocation = (Mesh && Mesh->DoesSocketExist(TEXT("ProjectileSpawn")))
		? Mesh->GetSocketLocation(TEXT("ProjectileSpawn"))
		: AvatarActor->GetActorLocation();

	const FRotator SpawnRotation = AvatarActor->GetActorRotation();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !DamageEffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddInstigator(AvatarActor, AvatarActor);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContext);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	SpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, CoreGAS::Combat::TAG_SetByCaller_DamageAmount, DamageAmount);

	if (ProjectileCount <= 1)
	{
		SpawnProjectile(SpawnLocation, SpawnRotation, SpecHandle);
	}
	else
	{
		for (int32 i = 0; i < ProjectileCount; ++i)
		{
			const float AngleOffset = -SpreadAngleDegrees / 2.f
				+ (ProjectileCount > 1 ? i * SpreadAngleDegrees / (ProjectileCount - 1) : 0.f);
			FRotator OffsetRotation = SpawnRotation;
			OffsetRotation.Yaw += AngleOffset;
			SpawnProjectile(SpawnLocation, OffsetRotation, SpecHandle);
		}
	}
}

void UCoreGameplayAbility_Projectile::SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation,
	const FGameplayEffectSpecHandle& SpecHandle)
{
	UWorld* World = GetWorld();
	if (!World || !ProjectileClass)
	{
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = AvatarActor ? Cast<APawn>(AvatarActor) : nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACoreProjectile* Projectile = World->SpawnActor<ACoreProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Projectile)
	{
		Projectile->DamageSpecHandle = SpecHandle;
	}
}
