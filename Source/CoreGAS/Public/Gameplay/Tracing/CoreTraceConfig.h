// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/HitResult.h"
#include "Engine/EngineTypes.h"
#include "CoreTraceConfig.generated.h"

UENUM(BlueprintType)
enum class ETraceShapeType : uint8
{
	Line,
	Sphere,
	Box,
	Capsule
};

UENUM(BlueprintType)
enum class ETraceStartLocation : uint8
{
	Avatar,
	Camera,
	Socket,
	Custom
};

UENUM(BlueprintType)
enum class ETraceEndLocation : uint8
{
	ForwardVector,
	Custom,
	Socket
};

UCLASS(BlueprintType)
class COREGAS_API UCoreTraceConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Shape")
	ETraceShapeType ShapeType = ETraceShapeType::Line;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Shape", meta = (EditCondition = "ShapeType == ETraceShapeType::Sphere || ShapeType == ETraceShapeType::Capsule", EditConditionHides))
	float ShapeRadius = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Shape", meta = (EditCondition = "ShapeType == ETraceShapeType::Box", EditConditionHides))
	FVector ShapeHalfExtent = FVector(25.f, 25.f, 25.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Shape", meta = (EditCondition = "ShapeType == ETraceShapeType::Capsule", EditConditionHides))
	float ShapeHalfHeight = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace")
	ETraceStartLocation StartLocation = ETraceStartLocation::Avatar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace")
	ETraceEndLocation EndLocation = ETraceEndLocation::ForwardVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace", meta = (EditCondition = "StartLocation == ETraceStartLocation::Socket", EditConditionHides))
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace", meta = (EditCondition = "EndLocation == ETraceEndLocation::Socket", EditConditionHides))
	FName EndSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace")
	float TraceDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace")
	bool bIgnoreInstigator = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace")
	bool bSingleTarget = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Debug")
	bool bDrawDebug = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Debug")
	FColor DebugColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Trace|Debug")
	float DebugDuration = 1.f;

	TArray<FHitResult> Execute(AActor* Owner, FVector CustomStart, FVector CustomEnd, AActor* Instigator = nullptr) const;

private:
	FVector GetStartLocation(AActor* Owner, FVector CustomStart) const;
	FVector GetEndLocation(AActor* Owner, FVector CustomEnd, FVector ResolvedStart) const;
	FCollisionShape BuildCollisionShape() const;
	void DrawDebugTrace(UWorld* World, FVector Start, FVector End, FRotator Direction) const;
};
