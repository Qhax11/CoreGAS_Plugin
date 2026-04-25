// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreWeaponBase.generated.h"

class UStaticMeshComponent;
class UCoreTraceConfig;

UCLASS()
class COREGAS_API ACoreWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ACoreWeaponBase();

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Weapon")
	FVector GetTraceStart() const;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Weapon")
	FVector GetTraceEnd() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Weapon")
	TObjectPtr<UCoreTraceConfig> TraceConfig;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Weapon")
	FName TraceStartSocket = "TraceStart";

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Weapon")
	FName TraceEndSocket = "TraceEnd";
};
