// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoreTraceOriginProvider.generated.h"

UINTERFACE(BlueprintType)
class COREGAS_API UCoreTraceOriginProvider : public UInterface
{
	GENERATED_BODY()
};

class COREGAS_API ICoreTraceOriginProvider
{
	GENERATED_BODY()

public:
	virtual FVector GetTraceOrigin() const { return FVector::ZeroVector; }
	virtual FRotator GetTraceDirection() const { return FRotator::ZeroRotator; }
};
