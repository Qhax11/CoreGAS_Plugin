// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "CoreAnimNotifyState_PhaseTag.generated.h"

UCLASS()
class COREGAS_API UCoreAnimNotifyState_PhaseTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "CoreGAS", Meta = (Categories = "CoreGAS.Ability.Phase"))
	FGameplayTag PhaseTag;
};
