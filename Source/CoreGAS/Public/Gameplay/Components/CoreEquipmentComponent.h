// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CoreEquipmentComponent.generated.h"

class ACoreWeaponBase;
class UCoreEquipmentData;

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoreEquipmentComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Equipment")
	void EquipWeaponByClass(TSubclassOf<ACoreWeaponBase> WeaponClass, FName AttachSocket, FGameplayTag SlotTag, FVector LocationOffset = FVector::ZeroVector, FRotator RotationOffset = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Equipment")
	void EquipWeapon(ACoreWeaponBase* Weapon, FName AttachSocket, FGameplayTag SlotTag, FVector LocationOffset = FVector::ZeroVector, FRotator RotationOffset = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Equipment")
	void UnequipWeapon(FGameplayTag SlotTag);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CoreGAS|Equipment")
	ACoreWeaponBase* GetWeaponBySlotTag(FGameplayTag SlotTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CoreGAS|Equipment")
	ACoreWeaponBase* GetFirstWeapon() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreGAS|Equipment")
	TObjectPtr<UCoreEquipmentData> EquipmentData;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "CoreGAS|Equipment")
	TMap<FGameplayTag, TObjectPtr<ACoreWeaponBase>> EquippedWeapons;
};
