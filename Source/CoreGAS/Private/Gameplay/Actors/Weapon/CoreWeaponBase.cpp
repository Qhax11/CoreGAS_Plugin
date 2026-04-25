// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Actors/Weapon/CoreWeaponBase.h"
#include "Components/StaticMeshComponent.h"

ACoreWeaponBase::ACoreWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

FVector ACoreWeaponBase::GetTraceStart() const
{
	if (WeaponMesh->DoesSocketExist(TraceStartSocket))
	{
		return WeaponMesh->GetSocketLocation(TraceStartSocket);
	}

	UE_LOG(LogTemp, Warning, TEXT("ACoreWeaponBase::GetTraceStart - Socket '%s' not found on WeaponMesh. Falling back to actor location."), *TraceStartSocket.ToString());
	return GetActorLocation();
}

FVector ACoreWeaponBase::GetTraceEnd() const
{
	if (WeaponMesh->DoesSocketExist(TraceEndSocket))
	{
		return WeaponMesh->GetSocketLocation(TraceEndSocket);
	}

	UE_LOG(LogTemp, Warning, TEXT("ACoreWeaponBase::GetTraceEnd - Socket '%s' not found on WeaponMesh. Falling back to actor location."), *TraceEndSocket.ToString());
	return GetActorLocation();
}
