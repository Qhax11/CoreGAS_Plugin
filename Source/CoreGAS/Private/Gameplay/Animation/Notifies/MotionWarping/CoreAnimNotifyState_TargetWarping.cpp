// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Animation/Notifies/MotionWarping/CoreAnimNotifyState_TargetWarping.h"
#include "Gameplay/Interfaces/ICoreCombatInterface.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCoreAnimNotifyState_TargetWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UpdateWarpTarget(MeshComp);
}

void UCoreAnimNotifyState_TargetWarping::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bUpdateWarpTargetOnTick) 
	{
		UpdateWarpTarget(MeshComp);
	}
}

#if WITH_EDITOR
bool UCoreAnimNotifyState_TargetWarping::CanEditChange(const FProperty* InProperty) const
{
	const bool ParentVal = Super::CanEditChange(InProperty);

	if (InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UCoreAnimNotifyState_TargetWarping, StopDistanceFromTarget))
	{
		const URootMotionModifier_Warp* WarpModifier = Cast<URootMotionModifier_Warp>(RootMotionModifier);
		return ParentVal && WarpModifier && WarpModifier->bWarpTranslation;
	}

	return ParentVal;
}
#endif

void UCoreAnimNotifyState_TargetWarping::UpdateWarpTarget(USkeletalMeshComponent* MeshComp)
{
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		return;
	}

	ICoreCombatInterface* CombatInterface = Cast<ICoreCombatInterface>(Owner);
	AActor* Target = CombatInterface ? CombatInterface->GetCurrentTarget() : nullptr;

	UMotionWarpingComponent* WarpComp = Owner->FindComponentByClass<UMotionWarpingComponent>();
	URootMotionModifier_Warp* WarpModifier = Cast<URootMotionModifier_Warp>(RootMotionModifier);
	if (!WarpComp || !WarpModifier)
	{
		return;
	}

	if (!IsValid(Target))
	{
		WarpComp->RemoveWarpTarget(WarpModifier->WarpTargetName);
		return;
	}

	const FVector OwnerLocation  = Owner->GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector ToTarget       = TargetLocation - OwnerLocation;
	const FVector WarpLocation   = TargetLocation - ToTarget.GetSafeNormal() * StopDistanceFromTarget;

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, TargetLocation);
	LookAtRotation.Pitch = 0.f;
	LookAtRotation.Roll  = 0.f;

	WarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(WarpModifier->WarpTargetName, WarpLocation, LookAtRotation);
}
