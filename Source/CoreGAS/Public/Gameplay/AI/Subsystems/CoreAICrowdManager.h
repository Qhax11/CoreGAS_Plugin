#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CoreAICrowdManager.generated.h"

class UCoreASCBase;

UCLASS()
class COREGAS_API UCoreAICrowdManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void RegisterEnemy(UCoreASCBase* ASC);
    void UnregisterEnemy(UCoreASCBase* ASC);

    bool RequestToken(UCoreASCBase* ASC);
    void ReleaseToken(UCoreASCBase* ASC);

private:
    TArray<TWeakObjectPtr<UCoreASCBase>> RegisteredEnemies;
    TArray<TWeakObjectPtr<UCoreASCBase>> ActiveAttackers;
    int32 MaxSimultaneousAttackers = 2;
};
