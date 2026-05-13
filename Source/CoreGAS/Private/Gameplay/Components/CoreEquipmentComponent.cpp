// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Components/CoreEquipmentComponent.h"
#include "Gameplay/Actors/Weapon/CoreWeaponBase.h"
#include "Gameplay/Data/CoreEquipmentData.h"
#include "Gameplay/Data/CoreWeaponSlotData.h"
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

	for (const FCoreWeaponSlotData& Entry : EquipmentData->DefaultWeapons)
	{
		EquipWeaponByClass(Entry.WeaponClass, Entry.AttachSocket, Entry.SlotTag, Entry.AttachLocationOffset, Entry.AttachRotationOffset);
	}
}

void UCoreEquipmentComponent::EquipWeaponByClass(TSubclassOf<ACoreWeaponBase> WeaponClass, FName AttachSocket, FGameplayTag SlotTag, FVector LocationOffset, FRotator RotationOffset)
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

	EquipWeapon(SpawnedWeapon, AttachSocket, SlotTag, LocationOffset, RotationOffset);
}

void UCoreEquipmentComponent::EquipWeapon(ACoreWeaponBase* Weapon, FName AttachSocket, FGameplayTag SlotTag, FVector LocationOffset, FRotator RotationOffset)
{
	if (!Weapon)
	{
		return;
	}

	if (EquippedWeapons.Contains(SlotTag))
	{
		UnequipWeapon(SlotTag);
	}

	USkeletalMeshComponent* OwnerMesh = GetOwner() ? GetOwner()->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
	if (!OwnerMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreEquipmentComponent::EquipWeapon - Owner has no SkeletalMeshComponent"));
		return;
	}

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(OwnerMesh, AttachRules, AttachSocket);
	Weapon->SetActorRelativeLocation(LocationOffset);
	Weapon->SetActorRelativeRotation(RotationOffset);

	EquippedWeapons.Add(SlotTag, Weapon);
}

void UCoreEquipmentComponent::UnequipWeapon(FGameplayTag SlotTag)
{
	TObjectPtr<ACoreWeaponBase>* Found = EquippedWeapons.Find(SlotTag);
	if (!Found || !(*Found))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCoreEquipmentComponent::UnequipWeapon - No weapon in slot '%s'"), *SlotTag.ToString());
		return;
	}

	(*Found)->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapons.Remove(SlotTag);
}

ACoreWeaponBase* UCoreEquipmentComponent::GetWeaponBySlotTag(FGameplayTag SlotTag) const
{
	const TObjectPtr<ACoreWeaponBase>* Found = EquippedWeapons.Find(SlotTag);
	return Found ? Found->Get() : nullptr;
}

ACoreWeaponBase* UCoreEquipmentComponent::GetFirstWeapon() const
{
	for (const TPair<FGameplayTag, TObjectPtr<ACoreWeaponBase>>& Pair : EquippedWeapons)
	{
		if (Pair.Value)
		{
			return Pair.Value.Get();
		}
	}
	return nullptr;
}
