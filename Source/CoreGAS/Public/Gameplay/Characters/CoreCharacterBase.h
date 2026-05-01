// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/Interfaces/CoreAbilitySystemInterface.h"
#include "MotionWarpingComponent.h"
#include "CoreCharacterBase.generated.h"

class UCoreEquipmentComponent;

UCLASS()
class COREGAS_API ACoreCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICoreAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACoreCharacterBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCoreASCBase* GetCoreAbilitySystemComponent() const override { return AbilitySystemComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void InitializeGAS() {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<class UCoreASCBase> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS|Components")
	TObjectPtr<UCoreEquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreGAS")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
};
