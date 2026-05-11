// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/CoreCharacterBase.h"
#include "CoreHeroBase.generated.h"

class UCoreHeroInputComponent;
class UCoreHeroGASDataComponent;
class UCoreHeroAttributeSet;
class UCoreTargetingComponent;

UCLASS()
class COREGAS_API ACoreHeroBase : public ACoreCharacterBase
{
	GENERATED_BODY()

public:
	ACoreHeroBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UCoreASCBase* GetCoreAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeGAS() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreHeroInputComponent> HeroInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreHeroGASDataComponent> HeroGASDataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreTargetingComponent> TargetingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Attributes")
	TObjectPtr<UCoreHeroAttributeSet> HeroAttributeSet;
};
