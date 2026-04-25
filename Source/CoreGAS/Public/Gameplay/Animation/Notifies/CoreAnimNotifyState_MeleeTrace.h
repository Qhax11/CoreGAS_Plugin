// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "CoreAnimNotifyState_MeleeTrace.generated.h"

UCLASS()
class COREGAS_API UCoreAnimNotifyState_MeleeTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCoreAnimNotifyState_MeleeTrace();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "CoreGAS|Melee")
	FGameplayTag TraceBeginTag;

	UPROPERTY(EditAnywhere, Category = "CoreGAS|Melee")
	FGameplayTag TraceEndTag;
};
