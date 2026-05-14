// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Animation/CoreAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCoreAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	OwnerMovementComponent = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
}

void UCoreAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerCharacter || !OwnerMovementComponent)
	{
		return;
	}

	Velocity    = OwnerMovementComponent->Velocity;
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bIsFalling  = OwnerMovementComponent->IsFalling();
	bShouldMove = GroundSpeed > 3.f;
}
