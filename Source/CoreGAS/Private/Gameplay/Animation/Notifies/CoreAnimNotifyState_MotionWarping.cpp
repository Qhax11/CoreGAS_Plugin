// Copyright (c) 2025/26 Synty Studios Limited. All rights reserved.

#include "Gameplay/Animation/Notifies/CoreAnimNotifyState_MotionWarping.h"
#include "Gameplay/Components/CoreTargetingComponent.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCoreAnimNotifyState_MotionWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (Owner)
	{
		UCoreTargetingComponent* TargetingComp = Owner->FindComponentByClass<UCoreTargetingComponent>();
		if (TargetingComp)
		{
			AActor* Target = TargetingComp->GetCurrentTarget();
			if (IsValid(Target))
			{
				UMotionWarpingComponent* WarpComp = Owner->FindComponentByClass<UMotionWarpingComponent>();
				URootMotionModifier_Warp* WarpModifier = Cast<URootMotionModifier_Warp>(RootMotionModifier);
				if (WarpComp && WarpModifier)
				{
					const FVector OwnerLocation  = Owner->GetActorLocation();
					const FVector TargetLocation = Target->GetActorLocation();
					const FVector ToTarget       = TargetLocation - OwnerLocation;
					const FVector WarpLocation   = TargetLocation - ToTarget.GetSafeNormal() * StopDistanceFromTarget;

					FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, TargetLocation);
					LookAtRotation.Pitch = 0.f;
					LookAtRotation.Roll  = 0.f;

					WarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(
						WarpModifier->WarpTargetName, WarpLocation, LookAtRotation);
				}
			}
			else
			{
				UMotionWarpingComponent* WarpComp = Owner->FindComponentByClass<UMotionWarpingComponent>();
				URootMotionModifier_Warp* WarpModifier = Cast<URootMotionModifier_Warp>(RootMotionModifier);
				if (WarpComp && WarpModifier)
				{
					WarpComp->RemoveWarpTarget(WarpModifier->WarpTargetName);
				}
			}
		}
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}
