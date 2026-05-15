#pragma once
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCoreAI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCoreAIDecision, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCoreAIMovement, Log, All);

// AI State log macro — Context.OwnerActor'ı otomatik prefix olarak ekler
#define CORE_AI_LOG(Category, Verbosity, Format, ...) \
    UE_LOG(Category, Verbosity, TEXT("[%s] " Format), \
        (Context.OwnerActor ? *Context.OwnerActor->GetName() : TEXT("NULL")), \
        ##__VA_ARGS__)