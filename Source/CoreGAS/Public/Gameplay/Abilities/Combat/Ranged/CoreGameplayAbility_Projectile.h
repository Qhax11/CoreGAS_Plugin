// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbility_Montage.h"
#include "CoreGameplayAbility_Projectile.generated.h"

class ACoreProjectile;
class UGameplayEffect;

UCLASS()
class COREGAS_API UCoreGameplayAbility_Projectile : public UCoreGameplayAbility_Montage
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_Projectile(); 

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Projectile")
	TSubclassOf<ACoreProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Projectile")
	float DamageAmount = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Projectile")
	int32 ProjectileCount = 1;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Projectile")
	float SpreadAngleDegrees = 0.f;

protected:
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

private:
	void SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation,
		const FGameplayEffectSpecHandle& SpecHandle);
};
