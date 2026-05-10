// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreTargetingComponent.generated.h"

UCLASS(ClassGroup=(CoreGAS), meta=(BlueprintSpawnableComponent))
class COREGAS_API UCoreTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoreTargetingComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Targeting")
	float TargetingInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	bool bDrawDebug = false;

	AActor* GetCurrentTarget() const { return CurrentTarget.Get(); }

private:
	void UpdateTarget();

	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget;
	FTimerHandle TargetingTimerHandle;
};
