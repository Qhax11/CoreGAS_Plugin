// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Actors/Weapon/CoreWeaponBase.h"
#include "Components/StaticMeshComponent.h"

ACoreWeaponBase::ACoreWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

FVector ACoreWeaponBase::GetTraceStart() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(TraceStartSocket))
	{
		return WeaponMesh->GetSocketLocation(TraceStartSocket);
	}

	UE_LOG(LogTemp, Warning, TEXT("ACoreWeaponBase::GetTraceStart - Socket '%s' not found on WeaponMesh"), *TraceStartSocket.ToString());
	return GetActorLocation();
}

FVector ACoreWeaponBase::GetTraceEnd() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(TraceEndSocket))
	{
		return WeaponMesh->GetSocketLocation(TraceEndSocket);
	}

	UE_LOG(LogTemp, Warning, TEXT("ACoreWeaponBase::GetTraceEnd - Socket '%s' not found on WeaponMesh"), *TraceEndSocket.ToString());
	return GetActorLocation();
}
