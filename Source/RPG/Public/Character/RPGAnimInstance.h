// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "RPGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	class ARPGCharacter* RPGCharacter;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	class UCharacterMovementComponent* RPGCharacterMovement;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

private:
	
};
