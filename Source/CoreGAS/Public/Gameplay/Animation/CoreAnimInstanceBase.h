// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CoreAnimInstanceBase.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class COREGAS_API UCoreAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS|Animation")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS|Animation")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS|Animation")
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly, Category = "CoreGAS|Animation")
	bool bIsFalling = false;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComponent;
};
