// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Components/CoreEquipmentComponent.h"
#include "Gameplay/Actors/Weapon/CoreWeaponBase.h"
#include "Gameplay/Data/CoreEquipmentData.h"
#include "Components/SkeletalMeshComponent.h"

UCoreEquipmentComponent::UCoreEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

void UCoreEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!EquipmentData)
	{
		return;
	}

	for (const TPair<FName, TSubclassOf<ACoreWeaponBase>>& Entry : EquipmentData->DefaultWeapons)
	{
		EquipWeaponByClass(Entry.Value, Entry.Key);
	}
}

void UCoreEquipmentComponent::EquipWeaponByClass(TSubclassOf<ACoreWeaponBase> WeaponClass, FName SocketName)
{
	if (!WeaponClass || !GetOwner() || !GetOwner()->GetWorld())
	{
		return;
	}

	ACoreWeaponBase* SpawnedWeapon = GetOwner()->GetWorld()->SpawnActor<ACoreWeaponBase>(WeaponClass);
	if (!SpawnedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreEquipmentComponent::EquipWeaponByClass - Failed to spawn weapon of class '%s'"), *WeaponClass->GetName());
		return;
	}

	EquipWeapon(SpawnedWeapon, SocketName);
}

void UCoreEquipmentComponent::EquipWeapon(ACoreWeaponBase* Weapon, FName SocketName)
{
	if (!Weapon)
	{
		return;
	}

	if (EquippedWeapons.Contains(SocketName))
	{
		UnequipWeapon(SocketName);
	}

	USkeletalMeshComponent* OwnerMesh = GetOwner() ? GetOwner()->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
	if (!OwnerMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreEquipmentComponent::EquipWeapon - Owner has no SkeletalMeshComponent"));
		return;
	}

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(OwnerMesh, AttachRules, SocketName);
	EquippedWeapons.Add(SocketName, Weapon);
}

void UCoreEquipmentComponent::UnequipWeapon(FName SocketName)
{
	TObjectPtr<ACoreWeaponBase>* Found = EquippedWeapons.Find(SocketName);
	if (!Found || !(*Found))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreEquipmentComponent::UnequipWeapon - No weapon in slot '%s'"), *SocketName.ToString());
		return;
	}

	(*Found)->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapons.Remove(SocketName);
}

ACoreWeaponBase* UCoreEquipmentComponent::GetWeaponAtSlot(FName SocketName) const
{
	const TObjectPtr<ACoreWeaponBase>* Found = EquippedWeapons.Find(SocketName);
	return Found ? Found->Get() : nullptr;
}

ACoreWeaponBase* UCoreEquipmentComponent::GetFirstWeapon() const
{
	for (const TPair<FName, TObjectPtr<ACoreWeaponBase>>& Pair : EquippedWeapons)
	{
		if (Pair.Value)
		{
			return Pair.Value.Get();
		}
	}
	return nullptr;
}
