// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "CoreProjectile.generated.h"

class USphereComponent;

UCLASS()
class COREGAS_API ACoreProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACoreProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS|Projectile")
	FGameplayEffectSpecHandle DamageSpecHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Projectile")
	FGameplayTagContainer TargetTags;

	UFUNCTION(BlueprintImplementableEvent, Category = "CoreGAS|Projectile")
	void BP_OnImpact(const FHitResult& HitResult, bool bHitTarget);

protected:
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
