#include "Gameplay/AI/Subsystems/CoreAICrowdManager.h"
#include "Gameplay/Settings/CoreCrowdSettings.h"
#include "Gameplay/Debug/CoreGameplayLog.h"

void UCoreAICrowdManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (const UCoreCrowdSettings* Settings = GetDefault<UCoreCrowdSettings>())
    {
        MaxSimultaneousAttackers = Settings->MaxSimultaneousAttackers;
    }
}

void UCoreAICrowdManager::RegisterEnemy(UCoreASCBase* ASC)
{
    if (!ASC) return;
    RegisteredEnemies.AddUnique(ASC);
}

void UCoreAICrowdManager::UnregisterEnemy(UCoreASCBase* ASC)
{
    if (!ASC) return;
    ReleaseToken(ASC);
    RegisteredEnemies.RemoveAll([ASC](const TWeakObjectPtr<UCoreASCBase>& Ptr) { return Ptr.Get() == ASC; });
}

bool UCoreAICrowdManager::RequestToken(UCoreASCBase* ASC)
{
    if (!ASC) return false;

    for (const TWeakObjectPtr<UCoreASCBase>& Ptr : ActiveAttackers)
    {
        if (Ptr.Get() == ASC) return true;
    }

    if (ActiveAttackers.Num() >= MaxSimultaneousAttackers) return false;

    ActiveAttackers.AddUnique(ASC);
    return true;
}

void UCoreAICrowdManager::ReleaseToken(UCoreASCBase* ASC)
{
    if (!ASC) return;
    ActiveAttackers.RemoveAll([ASC](const TWeakObjectPtr<UCoreASCBase>& Ptr) { return Ptr.Get() == ASC; });
}
