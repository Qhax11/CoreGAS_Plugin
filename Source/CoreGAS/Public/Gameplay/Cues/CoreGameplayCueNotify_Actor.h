// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "CoreGameplayCueNotify_Actor.generated.h"


UCLASS()
class COREGAS_API ACoreGameplayCueNotify_Actor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	bool OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	void OnActivated(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	void OnWhileActivated(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	void OnRemoved(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);
};
