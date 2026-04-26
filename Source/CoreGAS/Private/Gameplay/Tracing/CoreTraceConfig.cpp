// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Tracing/CoreTraceConfig.h"
#include "Gameplay/Tracing/CoreTraceOriginProvider.h"
#include "CollisionShape.h"
#include "CollisionQueryParams.h"
#include "Components/MeshComponent.h"
#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"
#include "Engine/World.h"

FVector UCoreTraceConfig::GetStartLocation(AActor* Owner, FVector CustomStart) const
{
	switch (StartLocation)
	{
	case ETraceStartLocation::Camera:
		if (ICoreTraceOriginProvider* Provider = Cast<ICoreTraceOriginProvider>(Owner))
		{
			return Provider->GetTraceOrigin();
		}
		return Owner->GetActorLocation();

	case ETraceStartLocation::Socket:
	{
		TArray<UMeshComponent*> MeshComponents;
		Owner->GetComponents<UMeshComponent>(MeshComponents);
		for (UMeshComponent* Mesh : MeshComponents)
		{
			if (Mesh && Mesh->DoesSocketExist(SocketName))
			{
				return Mesh->GetSocketLocation(SocketName);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("UCoreTraceConfig::GetStartLocation - Socket '%s' not found on %s"), *SocketName.ToString(), *Owner->GetName());
		return Owner->GetActorLocation();
	}

	case ETraceStartLocation::Custom:
		return CustomStart;

	default: // Avatar
		return Owner->GetActorLocation();
	}
}

FVector UCoreTraceConfig::GetEndLocation(AActor* Owner, FVector CustomEnd, FVector ResolvedStart) const
{
	switch (EndLocation)
	{
	case ETraceEndLocation::Custom:
		return CustomEnd;

	case ETraceEndLocation::Socket:
	{
		TArray<UMeshComponent*> MeshComponents;
		Owner->GetComponents<UMeshComponent>(MeshComponents);
		for (UMeshComponent* Mesh : MeshComponents)
		{
			if (Mesh && Mesh->DoesSocketExist(EndSocketName))
			{
				return Mesh->GetSocketLocation(EndSocketName);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("UCoreTraceConfig::GetEndLocation - Socket '%s' not found on %s"), *EndSocketName.ToString(), *Owner->GetName());
		return Owner->GetActorLocation();
	}

	default: // ForwardVector
		return ResolvedStart + Owner->GetActorForwardVector() * TraceDistance;
	}
}

FCollisionShape UCoreTraceConfig::BuildCollisionShape() const
{
	switch (ShapeType)
	{
	case ETraceShapeType::Sphere:
		return FCollisionShape::MakeSphere(ShapeRadius);
	case ETraceShapeType::Box:
		return FCollisionShape::MakeBox(ShapeHalfExtent);
	case ETraceShapeType::Capsule:
		return FCollisionShape::MakeCapsule(ShapeRadius, FMath::Max(ShapeHalfHeight, ShapeRadius));
	default: // Line
		return FCollisionShape();
	}
}

void UCoreTraceConfig::DrawDebugTrace(UWorld* World, FVector Start, FVector End, FRotator Direction) const
{
	if (!bDrawDebug)
	{
		return;
	}

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
		DrawDebugSweptSphere(World, Start, End, ShapeRadius, DebugColor, false, DebugDuration);
		break;
	}

	DrawDebugPoint(World, Start, 10.f, FColor::Green, false, DebugDuration);
	DrawDebugPoint(World, End, 10.f, FColor::Red, false, DebugDuration);
}

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

	const FVector Start = GetStartLocation(Owner, CustomStart);
	const FVector End = GetEndLocation(Owner, CustomEnd, Start);
	const FRotator Direction = (End - Start).Rotation();
	const FCollisionShape CollisionShape = BuildCollisionShape();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	if (bIgnoreSelf)
	{
		QueryParams.AddIgnoredActor(Owner);
	}

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

	DrawDebugTrace(World, Start, End, Direction);

	return HitResults;
}
