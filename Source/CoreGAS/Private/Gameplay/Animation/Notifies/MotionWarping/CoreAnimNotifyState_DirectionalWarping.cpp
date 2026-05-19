#include "Gameplay/Animation/Notifies/MotionWarping/CoreAnimNotifyState_DirectionalWarping.h"
#include "Gameplay/Tags/CoreAbilityTags.h"
#include "MotionWarpingComponent.h"
#include "Components/SkeletalMeshComponent.h"
void UCoreAnimNotifyState_DirectionalWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
    if (!Owner) return;

    UMotionWarpingComponent* WarpComp = Owner->FindComponentByClass<UMotionWarpingComponent>();
    URootMotionModifier_Warp* WarpModifier = Cast<URootMotionModifier_Warp>(RootMotionModifier);
    if (!WarpComp || !WarpModifier)
    {
        return;
    }

    FVector DirVec = Owner->GetActorForwardVector();
    if (DirectionTag == CoreGAS::Ability::TAG_Ability_Direction_Backward)
        DirVec = -Owner->GetActorForwardVector();
    else if (DirectionTag == CoreGAS::Ability::TAG_Ability_Direction_Right)
        DirVec = Owner->GetActorRightVector();
    else if (DirectionTag == CoreGAS::Ability::TAG_Ability_Direction_Left)
        DirVec = -Owner->GetActorRightVector();

    const FVector WarpLocation = Owner->GetActorLocation() + DirVec * WarpDistance;
    const FRotator WarpRotation = FRotator(0.f, Owner->GetActorRotation().Yaw, 0.f);

    WarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(WarpModifier->WarpTargetName, WarpLocation, WarpRotation);
}
