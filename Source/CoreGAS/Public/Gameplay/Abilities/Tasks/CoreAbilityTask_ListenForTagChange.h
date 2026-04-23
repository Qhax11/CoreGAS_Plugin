// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "CoreAbilityTask_ListenForTagChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoreTagChangeTaskDelegate, const FCoreTagChangeData&, ChangeData);

UCLASS()
class COREGAS_API UCoreAbilityTask_ListenForTagChange : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoreTagChangeTaskDelegate OnChanged;

	UFUNCTION(BlueprintCallable, Category = "CoreGAS|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UCoreAbilityTask_ListenForTagChange* ListenForTagChange(UGameplayAbility* OwningAbility, FGameplayTag Tag);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	FGameplayTag TrackedTag;
	FDelegateHandle DelegateHandle;
	int32 PreviousCount = 0;
};
