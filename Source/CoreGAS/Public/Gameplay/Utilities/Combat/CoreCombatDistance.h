#pragma once
#include "CoreMinimal.h"

enum class ECombatRangeResult : uint8
{
    TooClose,
    InRange,
    TooFar
};

namespace CoreCombat
{
    FORCEINLINE float GetDistance(const AActor* A, const AActor* B)
    {
        if (!IsValid(A) || !IsValid(B))
        {
            return TNumericLimits<float>::Max();
        }
        return FVector::Dist(A->GetActorLocation(), B->GetActorLocation());
    }

    FORCEINLINE bool IsInRange(const AActor* A, const AActor* B, float Range)
    {
        if (!IsValid(A) || !IsValid(B) || Range < 0.f)
        {
            return false;
        }
        return GetDistance(A, B) <= Range;
    }

    FORCEINLINE ECombatRangeResult EvaluateRange(const AActor* A, const AActor* B, float MinRange, float MaxRange)
    {
        if (!IsValid(A) || !IsValid(B))
        {
            return ECombatRangeResult::TooFar;
        }
        const float Distance = GetDistance(A, B);
        const float SafeMin = FMath::Max(0.f, MinRange);
        const float SafeMax = FMath::Max(SafeMin, MaxRange);
        if (Distance < SafeMin) return ECombatRangeResult::TooClose;
        if (Distance > SafeMax) return ECombatRangeResult::TooFar;
        return ECombatRangeResult::InRange;
    }
}
