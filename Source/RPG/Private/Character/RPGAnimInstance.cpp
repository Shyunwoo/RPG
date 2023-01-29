// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGAnimInstance.h"
#include "Character/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RPGCharacter = Cast<ARPGCharacter>(TryGetPawnOwner());
	if(RPGCharacter)
	{
		RPGCharacterMovement = RPGCharacter->GetCharacterMovement();
	}
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (RPGCharacter && RPGCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(RPGCharacterMovement->Velocity);
		bIsFalling = RPGCharacterMovement->IsFalling();

		CharacterState = RPGCharacter->GetCharacterState();
		ActionState = RPGCharacter->GetActionState();
		DeathPose = RPGCharacter->GetDeathPose();
	}
}
