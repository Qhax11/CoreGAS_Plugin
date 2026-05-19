#pragma once
#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "GameplayTagContainer.h"
#include "CoreAnimNotifyState_DirectionalWarping.generated.h"

UCLASS()
class COREGAS_API UCoreAnimNotifyState_DirectionalWarping : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Motion Warping", meta = (Categories = "CoreGAS.Ability.Direction"))
	FGameplayTag DirectionTag;

	UPROPERTY(EditAnywhere, Category = "Motion Warping")
	float WarpDistance = 500.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
