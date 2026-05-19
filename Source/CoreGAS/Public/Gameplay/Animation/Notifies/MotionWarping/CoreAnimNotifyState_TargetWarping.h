// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "CoreAnimNotifyState_TargetWarping.generated.h"

UCLASS()
class COREGAS_API UCoreAnimNotifyState_TargetWarping : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Motion Warping")
	float StopDistanceFromTarget = 200.f;

	UPROPERTY(EditAnywhere, Category = "Motion Warping")
	bool bUpdateWarpTargetOnTick = false;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

private:
	void UpdateWarpTarget(USkeletalMeshComponent* MeshComp);
};
