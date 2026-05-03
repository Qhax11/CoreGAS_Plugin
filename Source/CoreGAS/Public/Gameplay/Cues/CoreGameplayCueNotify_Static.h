// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "CoreGameplayCueNotify_Static.generated.h"


UCLASS()
class COREGAS_API UCoreGameplayCueNotify_Static : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	bool OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const;

	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	void OnActivated(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const;

	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	void OnWhileActivated(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const;

	UFUNCTION(BlueprintNativeEvent, Category = "CoreGAS|GameplayCue")
	void OnRemoved(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) const;
};
