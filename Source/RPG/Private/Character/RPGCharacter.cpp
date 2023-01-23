// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

ARPGCharacter::ARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject < UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), FName("HeadSocket"));

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), FName("HeadSocket"));
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARPGCharacter::MoveForward);
		PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ARPGCharacter::MoveRight);
		PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARPGCharacter::LookUp);
		PlayerInputComponent->BindAxis(FName("Turn"), this, &ARPGCharacter::Turn);

		PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ARPGCharacter::EKeyPressed);
		PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ARPGCharacter::Attack);
	}
}

void ARPGCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
	}
}

void ARPGCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::LookUp(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddControllerPitchInput(Value);
	}
}

void ARPGCharacter::Turn(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddControllerYawInput(Value);
	}
}

void ARPGCharacter::EKeyPressed()
{
	if (Controller)
	{
		AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if (OverlappingWeapon)
		{
			OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
			CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
			OverlappingItem = nullptr;
			EquippedWeapon = OverlappingWeapon;
		}
		else
		{
			if (CasDisarm())
			{
				PlayEquipMontage(FName("Unequip"));
				CharacterState = ECharacterState::ECS_Unequipped;
				ActionState = EActionState::EAS_EquippingWeapon;
			}
			else if(CanArm())
			{
				PlayEquipMontage(FName("Equip"));
				CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
				ActionState = EActionState::EAS_EquippingWeapon;
			}
		}
	}
}

void ARPGCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ARPGCharacter::Attack()
{
	if (Controller)
	{
		if (CanAttack())
		{
			PlayAttackMontage();
			ActionState = EActionState::EAS_Attcking;
		}
	}
}

void ARPGCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ARPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARPGCharacter::CasDisarm()
{
	return CharacterState != ECharacterState::ECS_Unequipped &&
		ActionState == EActionState::EAS_Unoccupied;
}

bool ARPGCharacter::CanArm()
{
	return CharacterState == ECharacterState::ECS_Unequipped &&
		ActionState == EActionState::EAS_Unoccupied &&
		EquippedWeapon;
}

void ARPGCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ARPGCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ARPGCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
