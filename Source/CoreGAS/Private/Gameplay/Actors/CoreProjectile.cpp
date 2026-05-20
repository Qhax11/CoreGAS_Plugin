// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Actors/CoreProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreGameplayCueTags.h"

ACoreProjectile::ACoreProjectile()
{
	bReplicates = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(15.f);
	CollisionComponent->SetCollisionObjectType(ECC_Projectile);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Hero, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bSweepCollision = true;

	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoreProjectile::OnProjectileOverlap);
}

void ACoreProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!ASC)
	{
		BP_OnImpact(SweepResult, false);
		Destroy();
		return;
	}

	if (!TargetTags.IsEmpty() && !ASC->HasAnyMatchingGameplayTags(TargetTags))
	{
		return;
	}

	if (DamageSpecHandle.IsValid())
	{
		if (ASC->HasMatchingGameplayTag(CoreGAS::Combat::TAG_State_Block))
		{
			FGameplayCueParameters CueParams;
			CueParams.Location = SweepResult.ImpactPoint;
			CueParams.Normal = SweepResult.ImpactNormal;
			CueParams.EffectContext = DamageSpecHandle.Data->GetContext();
			CueParams.EffectContext.AddHitResult(SweepResult);
			ASC->ExecuteGameplayCue(CoreGAS::GameplayCue::TAG_GameplayCue_Impact_Block, CueParams);
		}
		else
		{
			ASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
		}
	}

	BP_OnImpact(SweepResult, true);
	Destroy();
}
