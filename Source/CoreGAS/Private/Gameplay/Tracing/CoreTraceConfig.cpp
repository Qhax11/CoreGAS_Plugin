// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Tracing/CoreTraceConfig.h"
#include "Gameplay/Tracing/CoreTraceOriginProvider.h"
#include "CollisionShape.h"
#include "CollisionQueryParams.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

TArray<FHitResult> UCoreTraceConfig::Execute(AActor* Owner, FVector CustomStart, FVector CustomEnd) const
{
	if (!Owner)
	{
		return {};
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return {};
	}

	// Resolve start position and direction
	FVector Start;
	FRotator Direction;

	switch (StartLocation)
	{
	case ETraceStartLocation::Camera:
		if (ICoreTraceOriginProvider* Provider = Cast<ICoreTraceOriginProvider>(Owner))
		{
			Start = Provider->GetTraceOrigin();
			Direction = Provider->GetTraceDirection();
		}
		else
		{
			Start = Owner->GetActorLocation();
			Direction = Owner->GetActorRotation();
		}
		break;

	case ETraceStartLocation::Socket:
		if (USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			Start = Mesh->DoesSocketExist(SocketName) ? Mesh->GetSocketLocation(SocketName) : Owner->GetActorLocation();
		}
		else
		{
			Start = Owner->GetActorLocation();
		}
		Direction = Owner->GetActorRotation();
		break;

	case ETraceStartLocation::Custom:
		Start = CustomStart;
		Direction = Owner->GetActorRotation();
		break;

	default: // Avatar
		Start = Owner->GetActorLocation();
		Direction = Owner->GetActorRotation();
		break;
	}

	// Resolve end position
	FVector End;
	switch (EndLocation)
	{
	case ETraceEndLocation::Custom:
		End = CustomEnd;
		break;
	default: // ForwardVector
		End = Start + Direction.Vector() * TraceDistance;
		break;
	}

	// Build collision shape
	FCollisionShape CollisionShape;
	switch (ShapeType)
	{
	case ETraceShapeType::Sphere:
		CollisionShape = FCollisionShape::MakeSphere(ShapeRadius);
		break;
	case ETraceShapeType::Box:
		CollisionShape = FCollisionShape::MakeBox(ShapeHalfExtent);
		break;
	case ETraceShapeType::Capsule:
		CollisionShape = FCollisionShape::MakeCapsule(ShapeRadius, FMath::Max(ShapeHalfHeight, ShapeRadius));
		break;
	default: // Line
		CollisionShape = FCollisionShape();
		break;
	}

	// Build query params
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	if (bIgnoreSelf)
	{
		QueryParams.AddIgnoredActor(Owner);
	}

	// Perform trace
	TArray<FHitResult> HitResults;

	if (bSingleTarget)
	{
		FHitResult HitResult;
		bool bHit = false;
		if (CollisionShape.IsLine())
		{
			bHit = World->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, QueryParams);
		}
		else
		{
			bHit = World->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, TraceChannel, CollisionShape, QueryParams);
		}
		if (bHit)
		{
			HitResults.Add(HitResult);
		}
	}
	else
	{
		if (CollisionShape.IsLine())
		{
			World->LineTraceMultiByChannel(HitResults, Start, End, TraceChannel, QueryParams);
		}
		else
		{
			World->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, TraceChannel, CollisionShape, QueryParams);
		}
	}

	// Draw debug
	if (bDrawDebug)
	{
		switch (ShapeType)
		{
		case ETraceShapeType::Line:
			DrawDebugLine(World, Start, End, DebugColor, false, DebugDuration);
			break;
		case ETraceShapeType::Sphere:
			DrawDebugLine(World, Start, End, DebugColor, false, DebugDuration);
			DrawDebugSphere(World, Start, ShapeRadius, 16, DebugColor, false, DebugDuration);
			DrawDebugSphere(World, End, ShapeRadius, 16, DebugColor, false, DebugDuration);
			break;
		case ETraceShapeType::Box:
			DrawDebugLine(World, Start, End, DebugColor, false, DebugDuration);
			DrawDebugBox(World, Start, ShapeHalfExtent, Direction.Quaternion(), DebugColor, false, DebugDuration);
			DrawDebugBox(World, End, ShapeHalfExtent, Direction.Quaternion(), DebugColor, false, DebugDuration);
			break;
		case ETraceShapeType::Capsule:
		{
			const float ClampedHalfHeight = FMath::Max(ShapeHalfHeight, ShapeRadius);
			DrawDebugLine(World, Start, End, DebugColor, false, DebugDuration);
			DrawDebugCapsule(World, Start, ClampedHalfHeight, ShapeRadius, Direction.Quaternion(), DebugColor, false, DebugDuration);
			DrawDebugCapsule(World, End, ClampedHalfHeight, ShapeRadius, Direction.Quaternion(), DebugColor, false, DebugDuration);
			break;
		}
		}
	}

	return HitResults;
}
