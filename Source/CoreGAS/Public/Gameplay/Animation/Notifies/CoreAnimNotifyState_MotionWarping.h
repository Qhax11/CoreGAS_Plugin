// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "CoreAnimNotifyState_MotionWarping.generated.h"

UCLASS()
class COREGAS_API UCoreAnimNotifyState_MotionWarping : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Motion Warping")
	float StopDistanceFromTarget = 100.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
