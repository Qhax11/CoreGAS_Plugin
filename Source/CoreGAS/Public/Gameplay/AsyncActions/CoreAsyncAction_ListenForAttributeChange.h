// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "Gameplay/Types/CoreGASTypes.h"
#include "CoreAsyncAction_ListenForAttributeChange.generated.h"

class UCoreASCBase;

UCLASS()
class COREGAS_API UAsyncAction_ListenForAttributeChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoreOnAttributeChanged OnChanged;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "CoreGAS")
	static UAsyncAction_ListenForAttributeChange* ListenForAttributeChange(UCoreASCBase* ASC, FGameplayAttribute Attribute);

	virtual void Activate() override;

private:
	TWeakObjectPtr<UCoreASCBase> CachedASC;
	FGameplayAttribute CachedAttribute;
};
