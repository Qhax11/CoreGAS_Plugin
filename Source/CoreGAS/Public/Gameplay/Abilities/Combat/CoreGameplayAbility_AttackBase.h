// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/CoreGameplayAbility_Montage.h"
#include "CoreGameplayAbility_AttackBase.generated.h"

UCLASS()
class COREGAS_API UCoreGameplayAbility_AttackBase : public UCoreGameplayAbility_Montage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MinRange = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MaxRange = 150.f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|AttackBase")
	TArray<TSubclassOf<UGameplayEffect>> HitEffects;

	void ApplyHitToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FHitResult& Hit);

};
