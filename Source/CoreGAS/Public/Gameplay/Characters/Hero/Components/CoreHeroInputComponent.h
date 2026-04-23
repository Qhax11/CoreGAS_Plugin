// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "CoreHeroInputComponent.generated.h"

class UInputMappingContext;
class UCoreASCBase;

UCLASS()
class COREGAS_API UCoreHeroInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeInputComponent(UCoreASCBase* ASC, UInputComponent* InputComponent, APlayerController* PC);
	void BindAbilityInput(UInputAction* Action, const FGameplayTag& InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "CoreGAS|Input")
	int32 MappingPriority = 0;

private:
	TObjectPtr<UCoreASCBase> CachedASC;
	TObjectPtr<UInputComponent> CachedInputComponent;
};
