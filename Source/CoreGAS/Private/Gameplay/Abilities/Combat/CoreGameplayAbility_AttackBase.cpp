// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Abilities/Combat/CoreGameplayAbility_AttackBase.h"
#include "Gameplay/Tags/CoreCombatTags.h"
#include "Gameplay/Tags/CoreCharacterTags.h"
#include "Gameplay/Tags/CoreGameplayCueTags.h"
#include "AbilitySystemComponent.h"

void UCoreGameplayAbility_AttackBase::ApplyHitToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FHitResult& Hit)
{
    const bool bSourceIsEnemy = SourceASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Entity_Enemy);

    // Enemy don't hit to another enemy
    if (bSourceIsEnemy && TargetASC->HasMatchingGameplayTag(CoreGAS::Character::TAG_Entity_Enemy))
    {
        return;
    }

    // Block check
    if (TargetASC->HasMatchingGameplayTag(CoreGAS::Combat::TAG_State_Block))
    {
        FGameplayCueParameters CueParams;
        CueParams.Location = Hit.ImpactPoint;
        CueParams.Normal = Hit.ImpactNormal;
        CueParams.EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
        CueParams.EffectContext.AddHitResult(Hit);

        TargetASC->ExecuteGameplayCue(CoreGAS::GameplayCue::TAG_GameplayCue_Impact_Block, CueParams);
        return;
    }

    // Normal damage flow
    for (const TSubclassOf<UGameplayEffect>& EffectClass : HitEffects)
    {
        if (!EffectClass) continue;

        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
        SpecHandle.Data->GetContext().AddHitResult(Hit, true);

        if (SpecHandle.IsValid())
        {
            SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
        }
    }
}
