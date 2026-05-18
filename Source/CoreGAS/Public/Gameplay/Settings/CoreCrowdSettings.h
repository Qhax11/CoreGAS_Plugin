#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CoreCrowdSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Core Crowd Settings"))
class COREGAS_API UCoreCrowdSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, Category = "Crowd", meta = (ClampMin = 1))
    int32 MaxSimultaneousAttackers = 2;
};
