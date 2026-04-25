// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Tracing/CoreTraceConfig.h"
#include "Gameplay/Tracing/CoreTraceOriginProvider.h"
#include "CollisionShape.h"
#include "CollisionQueryParams.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"
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

	// Resolve start position
	FVector Start;
	FRotator StartRotation;

	switch (StartLocation)
	{
	case ETraceStartLocation::Camera:
		if (ICoreTraceOriginProvider* Provider = Cast<ICoreTraceOriginProvider>(Owner))
		{
			Start = Provider->GetTraceOrigin();
			StartRotation = Provider->GetTraceDirection();
		}
		else
		{
			Start = Owner->GetActorLocation();
			StartRotation = Owner->GetActorRotation();
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
		StartRotation = Owner->GetActorRotation();
		break;

	case ETraceStartLocation::Custom:
		Start = CustomStart;
		StartRotation = Owner->GetActorRotation();
		break;

	default: // Avatar
		Start = Owner->GetActorLocation();
		StartRotation = Owner->GetActorRotation();
		break;
	}

	// Resolve end position
	FVector End;
	switch (EndLocation)
	{
	case ETraceEndLocation::Custom:
		End = CustomEnd;
		break;
	case ETraceEndLocation::Socket:
	{
		USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (Mesh && Mesh->DoesSocketExist(EndSocketName))
		{
			End = Mesh->GetSocketLocation(EndSocketName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UCoreTraceConfig::Execute - EndSocketName '%s' not found on %s, falling back to owner location."), *EndSocketName.ToString(), *Owner->GetName());
			End = Owner->GetActorLocation();
		}
		break;
	}
	default: // ForwardVector
		End = Start + StartRotation.Vector() * TraceDistance;
		break;
	}

	// Direction derived from the resolved trace path
	FRotator Direction = (End - Start).Rotation();

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
			bHit = World->SweepSingleByChannel(HitResult, Start, End, Direction.Quaternion(), TraceChannel, CollisionShape, QueryParams);
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
			World->SweepMultiByChannel(HitResults, Start, End, Direction.Quaternion(), TraceChannel, CollisionShape, QueryParams);
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
			DrawDebugSweptSphere(World, Start, End, ShapeRadius, DebugColor, false, DebugDuration);
			break;
		case ETraceShapeType::Box:
			DrawDebugSweptBox(World, Start, End, Direction, ShapeHalfExtent, DebugColor, false, DebugDuration);
			break;
		case ETraceShapeType::Capsule:
		{
			const float ClampedHalfHeight = FMath::Max(ShapeHalfHeight, ShapeRadius);
			DrawDebugCapsule(World, (Start + End) * 0.5f, ClampedHalfHeight, ShapeRadius, Direction.Quaternion(), DebugColor, false, DebugDuration);
			break;
		}
		}
	}

	return HitResults;
}
