// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AttributeSet.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "CoreAbilityTask_ListenForAttributeChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoreAttributeChangeTaskDelegate, const FCoreAttributeChangeData&, ChangeData);

UCLASS()
class COREGAS_API UCoreAbilityTask_ListenForAttributeChange : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoreAttributeChangeTaskDelegate OnChanged;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UCoreAbilityTask_ListenForAttributeChange* ListenForAttributeChange(UGameplayAbility* OwningAbility, FGameplayAttribute Attribute);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	FGameplayAttribute TrackedAttribute;
	FDelegateHandle DelegateHandle;
};
